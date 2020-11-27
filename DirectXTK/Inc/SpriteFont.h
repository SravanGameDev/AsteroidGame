//--------------------------------------------------------------------------------------
// File: SpriteFont.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#pragma once

#include <d3d11.h>
#include <DirectXColors.h>
#include <memory>

namespace DirectX
{
    class ISpriteFontRenderer
    {
    public:
        virtual ~ISpriteFontRenderer();
        virtual void DrawGlyph(XMVECTOR position, const RECT *uvs) = 0;
    };

    class SpriteFont
    {
    public:
        struct Glyph;

        SpriteFont(_In_ ID3D11Device* device, _In_z_ wchar_t const* fileName, bool forceSRGB = false);
        SpriteFont(_In_ ID3D11Device* device, _In_reads_bytes_(dataSize) uint8_t const* dataBlob, _In_ size_t dataSize, bool forceSRGB = false);
        SpriteFont(_In_ ID3D11ShaderResourceView* texture, _In_reads_(glyphCount) Glyph const* glyphs, _In_ size_t glyphCount, _In_ float lineSpacing);

        SpriteFont(SpriteFont&& moveFrom) noexcept;
        SpriteFont& operator= (SpriteFont&& moveFrom) noexcept;

        SpriteFont(SpriteFont const&) = delete;
        SpriteFont& operator= (SpriteFont const&) = delete;

        virtual ~SpriteFont();

        // Wide-character / UTF-16LE
        void XM_CALLCONV DrawString(_In_ ISpriteFontRenderer* spriteBatch, _In_z_ wchar_t const* text, FXMVECTOR position, FXMVECTOR color) const;

        XMVECTOR XM_CALLCONV MeasureString(_In_z_ wchar_t const* text) const;

        RECT __cdecl MeasureDrawBounds(_In_z_ wchar_t const* text, XMFLOAT2 const& position) const;
        RECT XM_CALLCONV MeasureDrawBounds(_In_z_ wchar_t const* text, FXMVECTOR position) const;

        // UTF-8
        void XM_CALLCONV DrawString(_In_ ISpriteFontRenderer* spriteBatch, _In_z_ char const* text, FXMVECTOR position, FXMVECTOR color) const;

        XMVECTOR XM_CALLCONV MeasureString(_In_z_ char const* text) const;

        RECT __cdecl MeasureDrawBounds(_In_z_ char const* text, XMFLOAT2 const& position) const;
        RECT XM_CALLCONV MeasureDrawBounds(_In_z_ char const* text, FXMVECTOR position) const;

        // Spacing properties
        float __cdecl GetLineSpacing() const;
        void __cdecl SetLineSpacing(float spacing);

        // Font properties
        wchar_t __cdecl GetDefaultCharacter() const;
        void __cdecl SetDefaultCharacter(wchar_t character);

        bool __cdecl ContainsCharacter(wchar_t character) const;

        // Custom layout/rendering
        Glyph const* __cdecl FindGlyph(wchar_t character) const;
        void __cdecl GetSpriteSheet(ID3D11ShaderResourceView** texture) const;

        // Describes a single character glyph.
        struct Glyph
        {
            uint32_t Character;
            RECT Subrect;
            float XOffset;
            float YOffset;
            float XAdvance;
        };


    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;

        static const XMFLOAT2 Float2Zero;
    };
}
