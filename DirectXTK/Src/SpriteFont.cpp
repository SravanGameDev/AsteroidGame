//--------------------------------------------------------------------------------------
// File: SpriteFont.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#include "pch.h"

#include <algorithm>
#include <vector>

#include "SpriteFont.h"
#include "DirectXHelpers.h"
#include "BinaryReader.h"
#include "LoaderHelpers.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;


// Internal SpriteFont implementation class.
class SpriteFont::Impl
{
public:
    Impl(_In_ ID3D11Device* device, _In_ BinaryReader* reader, bool forceSRGB);
    Impl(_In_ ID3D11ShaderResourceView* texture, _In_reads_(glyphCount) Glyph const* glyphs, _In_ size_t glyphCount, _In_ float lineSpacing);

    Glyph const* FindGlyph(wchar_t character) const;

    void SetDefaultCharacter(wchar_t character);

    template<typename TAction>
    void ForEachGlyph(_In_z_ wchar_t const* text, TAction action) const;

    const wchar_t* ConvertUTF8(_In_z_ const char *text);

    // Fields.
    ComPtr<ID3D11ShaderResourceView> texture;
    ComPtr<ID3D11SamplerState> sampler;
    std::vector<Glyph> glyphs;
    Glyph const* defaultGlyph;
    float lineSpacing;

private:
    size_t utfBufferSize;
    std::unique_ptr<wchar_t[]> utfBuffer;
};


// Constants.
const XMFLOAT2 SpriteFont::Float2Zero(0, 0);

static const char spriteFontMagic[] = "DXTKfont";


// Comparison operators make our sorted glyph vector work with std::binary_search and lower_bound.
namespace DirectX
{
    static inline bool operator< (SpriteFont::Glyph const& left, SpriteFont::Glyph const& right)
    {
        return left.Character < right.Character;
    }

    static inline bool operator< (wchar_t left, SpriteFont::Glyph const& right)
    {
        return left < right.Character;
    }

    static inline bool operator< (SpriteFont::Glyph const& left, wchar_t right)
    {
        return left.Character < right;
    }
}

namespace DirectX
{
    ISpriteFontRenderer::~ISpriteFontRenderer()
    {
    }
}

// Reads a SpriteFont from the binary format created by the MakeSpriteFont utility.
SpriteFont::Impl::Impl(_In_ ID3D11Device* device, _In_ BinaryReader* reader, bool forceSRGB) :
    defaultGlyph(nullptr),
    utfBufferSize(0)
{
    // Validate the header.
    for (char const* magic = spriteFontMagic; *magic; magic++)
    {
        if (reader->Read<uint8_t>() != *magic)
        {
            DebugTrace("ERROR: SpriteFont provided with an invalid .spritefont file\n");
            throw std::exception("Not a MakeSpriteFont output binary");
        }
    }

    // Read the glyph data.
    auto glyphCount = reader->Read<uint32_t>();
    auto glyphData = reader->ReadArray<Glyph>(glyphCount);

    glyphs.assign(glyphData, glyphData + glyphCount);

    // Read font properties.
    lineSpacing = reader->Read<float>();

    SetDefaultCharacter(static_cast<wchar_t>(reader->Read<uint32_t>()));

    // Read the texture data.
    auto textureWidth = reader->Read<uint32_t>();
    auto textureHeight = reader->Read<uint32_t>();
    auto textureFormat = reader->Read<DXGI_FORMAT>();
    auto textureStride = reader->Read<uint32_t>();
    auto textureRows = reader->Read<uint32_t>();
    auto textureData = reader->ReadArray<uint8_t>(size_t(textureStride) * size_t(textureRows));

    if (forceSRGB)
    {
        textureFormat = LoaderHelpers::MakeSRGB(textureFormat);
    }

    // Create the D3D texture.
    CD3D11_TEXTURE2D_DESC textureDesc(textureFormat, textureWidth, textureHeight, 1, 1, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);
    CD3D11_SHADER_RESOURCE_VIEW_DESC viewDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureFormat);
    D3D11_SUBRESOURCE_DATA initData = { textureData, textureStride, 0 };
    ComPtr<ID3D11Texture2D> texture2D;

    ThrowIfFailed(
        device->CreateTexture2D(&textureDesc, &initData, &texture2D)
    );

    ThrowIfFailed(
        device->CreateShaderResourceView(texture2D.Get(), &viewDesc, &texture)
    );

    SetDebugObjectName(texture.Get(), "DirectXTK:SpriteFont");
    SetDebugObjectName(texture2D.Get(), "DirectXTK:SpriteFont");
}


// Constructs a SpriteFont from arbitrary user specified glyph data.
_Use_decl_annotations_
SpriteFont::Impl::Impl(ID3D11ShaderResourceView* texture, Glyph const* glyphs, size_t glyphCount, float lineSpacing)
    : texture(texture),
    glyphs(glyphs, glyphs + glyphCount),
    defaultGlyph(nullptr),
    lineSpacing(lineSpacing),
    utfBufferSize(0)
{
    if (!std::is_sorted(glyphs, glyphs + glyphCount))
    {
        throw std::exception("Glyphs must be in ascending codepoint order");
    }
}


// Looks up the requested glyph, falling back to the default character if it is not in the font.
SpriteFont::Glyph const* SpriteFont::Impl::FindGlyph(wchar_t character) const
{
    auto glyph = std::lower_bound(glyphs.begin(), glyphs.end(), character);

    if (glyph != glyphs.end() && glyph->Character == character)
    {
        return &*glyph;
    }

    if (defaultGlyph)
    {
        return defaultGlyph;
    }

    DebugTrace("ERROR: SpriteFont encountered a character not in the font (%u, %C), and no default glyph was provided\n", character, character);
    throw std::exception("Character not in font");
}


// Sets the missing-character fallback glyph.
void SpriteFont::Impl::SetDefaultCharacter(wchar_t character)
{
    defaultGlyph = nullptr;

    if (character)
    {
        defaultGlyph = FindGlyph(character);
    }
}


// The core glyph layout algorithm, shared between DrawString and MeasureString.
template<typename TAction>
void SpriteFont::Impl::ForEachGlyph(_In_z_ wchar_t const* text, TAction action) const
{
    float x = 0;
    float y = 0;

    for (; *text; text++)
    {
        wchar_t character = *text;

        switch (character)
        {
            case '\r':
                // Skip carriage returns.
                continue;

            case '\n':
                // New line.
                x = 0;
                y += lineSpacing;
                break;

            default:
                // Output this character.
                auto glyph = FindGlyph(character);

                x += glyph->XOffset;

                if (x < 0)
                    x = 0;

                float advance = glyph->Subrect.right - glyph->Subrect.left + glyph->XAdvance;

                if (!iswspace(character)
                    || ((glyph->Subrect.right - glyph->Subrect.left) > 1)
                    || ((glyph->Subrect.bottom - glyph->Subrect.top) > 1))
                {
                    action(glyph, x, y, advance);
                }

                x += advance;
                break;
        }
    }
}


const wchar_t* SpriteFont::Impl::ConvertUTF8(_In_z_ const char *text)
{
    if (!utfBuffer)
    {
        utfBufferSize = 1024;
        utfBuffer.reset(new wchar_t[1024]);
    }

    int result = MultiByteToWideChar(CP_UTF8, 0, text, -1, utfBuffer.get(), static_cast<int>(utfBufferSize));
    if (!result && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
    {
        // Compute required buffer size
        result = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
        utfBufferSize = AlignUp(result, 1024);
        utfBuffer.reset(new wchar_t[utfBufferSize]);

        // Retry conversion
        result = MultiByteToWideChar(CP_UTF8, 0, text, -1, utfBuffer.get(), static_cast<int>(utfBufferSize));
    }

    if (!result)
    {
        DebugTrace("ERROR: MultiByteToWideChar failed with error %u.\n", GetLastError());
        throw std::exception("MultiByteToWideChar");
    }

    return utfBuffer.get();
}


// Construct from a binary file created by the MakeSpriteFont utility.
SpriteFont::SpriteFont(_In_ ID3D11Device* device, _In_z_ wchar_t const* fileName, bool forceSRGB)
{
    BinaryReader reader(fileName);

    pImpl = std::make_unique<Impl>(device, &reader, forceSRGB);
}


// Construct from a binary blob created by the MakeSpriteFont utility and already loaded into memory.
_Use_decl_annotations_
SpriteFont::SpriteFont(ID3D11Device* device, uint8_t const* dataBlob, size_t dataSize, bool forceSRGB)
{
    BinaryReader reader(dataBlob, dataSize);

    pImpl = std::make_unique<Impl>(device, &reader, forceSRGB);
}


// Construct from arbitrary user specified glyph data (for those not using the MakeSpriteFont utility).
_Use_decl_annotations_
SpriteFont::SpriteFont(ID3D11ShaderResourceView* texture, Glyph const* glyphs, size_t glyphCount, float lineSpacing)
    : pImpl(std::make_unique<Impl>(texture, glyphs, glyphCount, lineSpacing))
{
}


// Move constructor.
SpriteFont::SpriteFont(SpriteFont&& moveFrom) noexcept
    : pImpl(std::move(moveFrom.pImpl))
{
}


// Move assignment.
SpriteFont& SpriteFont::operator= (SpriteFont&& moveFrom) noexcept
{
    pImpl = std::move(moveFrom.pImpl);
    return *this;
}


// Public destructor.
SpriteFont::~SpriteFont()
{
}


// Wide-character / UTF-16LE
void XM_CALLCONV SpriteFont::DrawString(_In_ ISpriteFontRenderer* spriteBatch, _In_z_ wchar_t const* text, FXMVECTOR position, FXMVECTOR color) const
{
    // Draw each character in turn.
    pImpl->ForEachGlyph(text, [&](Glyph const* glyph, float x, float y, float advance)
    {
        UNREFERENCED_PARAMETER(advance);

        XMVECTOR offsetPosition = XMVectorAdd(position, XMVectorSet(x, y + glyph->YOffset, 0.0f, 0.0f));

        spriteBatch->DrawGlyph(offsetPosition, &glyph->Subrect);
    });
}


XMVECTOR XM_CALLCONV SpriteFont::MeasureString(_In_z_ wchar_t const* text) const
{
    XMVECTOR result = XMVectorZero();

    pImpl->ForEachGlyph(text, [&](Glyph const* glyph, float x, float y, float advance)
    {
        UNREFERENCED_PARAMETER(advance);

        auto w = static_cast<float>(glyph->Subrect.right - glyph->Subrect.left);
        auto h = static_cast<float>(glyph->Subrect.bottom - glyph->Subrect.top) + glyph->YOffset;

        h = std::max(h, pImpl->lineSpacing);

        result = XMVectorMax(result, XMVectorSet(x + w, y + h, 0, 0));
    });

    return result;
}


RECT SpriteFont::MeasureDrawBounds(_In_z_ wchar_t const* text, XMFLOAT2 const& position) const
{
    RECT result = { LONG_MAX, LONG_MAX, 0, 0 };

    pImpl->ForEachGlyph(text, [&](Glyph const* glyph, float x, float y, float advance)
    {
        auto w = static_cast<float>(glyph->Subrect.right - glyph->Subrect.left);
        auto h = static_cast<float>(glyph->Subrect.bottom - glyph->Subrect.top);

        float minX = position.x + x;
        float minY = position.y + y + glyph->YOffset;

        float maxX = std::max(minX + advance, minX + w);
        float maxY = minY + h;

        if (minX < result.left)
            result.left = long(minX);

        if (minY < result.top)
            result.top = long(minY);

        if (result.right < maxX)
            result.right = long(maxX);

        if (result.bottom < maxY)
            result.bottom = long(maxY);
    });

    if (result.left == LONG_MAX)
    {
        result.left = 0;
        result.top = 0;
    }

    return result;
}


RECT XM_CALLCONV SpriteFont::MeasureDrawBounds(_In_z_ wchar_t const* text, FXMVECTOR position) const
{
    XMFLOAT2 pos;
    XMStoreFloat2(&pos, position);

    return MeasureDrawBounds(text, pos);
}


// UTF-8
void XM_CALLCONV SpriteFont::DrawString(_In_ ISpriteFontRenderer* spriteBatch, _In_z_ char const* text, FXMVECTOR position, FXMVECTOR color) const
{
    DrawString(spriteBatch, pImpl->ConvertUTF8(text), position, color);
}


XMVECTOR XM_CALLCONV SpriteFont::MeasureString(_In_z_ char const* text) const
{
    return MeasureString(pImpl->ConvertUTF8(text));
}


RECT SpriteFont::MeasureDrawBounds(_In_z_ char const* text, XMFLOAT2 const& position) const
{
    return MeasureDrawBounds(pImpl->ConvertUTF8(text), position);
}


RECT XM_CALLCONV SpriteFont::MeasureDrawBounds(_In_z_ char const* text, FXMVECTOR position) const
{
    XMFLOAT2 pos;
    XMStoreFloat2(&pos, position);

    return MeasureDrawBounds(pImpl->ConvertUTF8(text), pos);
}


// Spacing properties
float SpriteFont::GetLineSpacing() const
{
    return pImpl->lineSpacing;
}


void SpriteFont::SetLineSpacing(float spacing)
{
    pImpl->lineSpacing = spacing;
}


// Font properties
wchar_t SpriteFont::GetDefaultCharacter() const
{
    return static_cast<wchar_t>(pImpl->defaultGlyph ? pImpl->defaultGlyph->Character : 0);
}


void SpriteFont::SetDefaultCharacter(wchar_t character)
{
    pImpl->SetDefaultCharacter(character);
}


bool SpriteFont::ContainsCharacter(wchar_t character) const
{
    return std::binary_search(pImpl->glyphs.begin(), pImpl->glyphs.end(), character);
}


// Custom layout/rendering
SpriteFont::Glyph const* SpriteFont::FindGlyph(wchar_t character) const
{
    return pImpl->FindGlyph(character);
}


void SpriteFont::GetSpriteSheet(ID3D11ShaderResourceView** texture) const
{
    if (!texture)
        return;

    ThrowIfFailed(pImpl->texture.CopyTo(texture));
}
