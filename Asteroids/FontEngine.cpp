#include "FontEngine.h"
#include "ResourceLoader.h"
#include "resource.h"
#include "DynamicVertexBuffers.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "MatrixBuffer.h"
#include "SpriteFontVertex.h"
#include "SpriteFontRenderer.h"
#include <SpriteFont.h>

FontEngine::FontEngine(const InitialisationParams &initParams) :
	d3dDeviceContext_(initParams.d3dDeviceContext),
	vertexBuffers_(initParams.vertexBuffers),
	vertexShader_(initParams.vertexShader),
	pixelShader_(initParams.pixelShader),
	modelViewProjection_(initParams.modelViewProjection),
	textureSampler_(initParams.textureSampler),
	fonts_(initParams.fonts)
{
	 XMStoreFloat4x4(&projectionMatrix_, XMMatrixOrthographicOffCenterLH(
		0.0f,
		800.0f,
		600.0f,
		0.0f,
		-100.0f,
		100.0f));
}

FontEngine::~FontEngine()
{
}

FontEngine *FontEngine::CreateFontEngine(ResourceLoader *resources,
	ID3D11Device *d3dDevice,
	ID3D11DeviceContext *d3dDeviceContext)
{
	const unsigned int MAXIMUM_GLYPHS = 64 * 1024;
	const unsigned int MAXIMUM_FONT_VERTICES = 6 * MAXIMUM_GLYPHS;

	bool resourcesLoaded = true;

	ResourceLoader::Resource vertexShaderResource;
	resourcesLoaded &= resources->LoadResource(IDR_VERTEX_SHADER_SPRITEFONT, &vertexShaderResource);

	ResourceLoader::Resource pixelShaderResource;
	resourcesLoaded &= resources->LoadResource(IDR_PIXEL_SHADER_SPRITEFONT, &pixelShaderResource);

	ResourceLoader::Resource fontResources[3];
	resourcesLoaded &= resources->LoadResource(IDR_ARIAL_12_SPRITEFONT, &fontResources[0]);
	resourcesLoaded &= resources->LoadResource(IDR_ARIAL_24_SPRITEFONT, &fontResources[1]);
	resourcesLoaded &= resources->LoadResource(IDR_ARIAL_36_SPRITEFONT, &fontResources[2]);

	InitialisationParams engineParams;

	engineParams.vertexBuffers = DynamicVertexBuffers::CreateDynamicVertexBuffers<SpriteFontVertex>(MAXIMUM_FONT_VERTICES,
		2,
		d3dDevice);
	engineParams.modelViewProjection = MatrixBuffer::CreateMatrixBuffer(d3dDevice);

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.resize(3);

	vertexLayout[0].SemanticName = "POSITION";
	vertexLayout[0].SemanticIndex = 0;
	vertexLayout[0].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexLayout[0].InputSlot = 0;
	vertexLayout[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[0].InstanceDataStepRate = 0;

	vertexLayout[1].SemanticName = "TEXCOORD";
	vertexLayout[1].SemanticIndex = 0;
	vertexLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexLayout[1].InputSlot = 0;
	vertexLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[1].InstanceDataStepRate = 0;

	vertexLayout[2].SemanticName = "COLOR";
	vertexLayout[2].SemanticIndex = 0;
	vertexLayout[2].Format = DXGI_FORMAT_R8G8B8A8_UINT;
	vertexLayout[2].InputSlot = 0;
	vertexLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[2].InstanceDataStepRate = 0;

	engineParams.vertexShader = VertexShader::CreateVertexShader(
		vertexShaderResource.data,
		vertexShaderResource.size,
		d3dDevice,
		vertexLayout);

	engineParams.pixelShader = PixelShader::CreatePixelShader(
		pixelShaderResource.data,
		pixelShaderResource.size,
		d3dDevice);

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	d3dDevice->CreateSamplerState(&samplerDesc, &engineParams.textureSampler);

	engineParams.fonts[FONT_TYPE_SMALL] = CreateFont(d3dDevice,
		static_cast<uint8_t *>(fontResources[0].data),
		fontResources[0].size);
	engineParams.fonts[FONT_TYPE_MEDIUM] = CreateFont(d3dDevice,
		static_cast<uint8_t *>(fontResources[1].data),
		fontResources[1].size);
	engineParams.fonts[FONT_TYPE_LARGE] = CreateFont(d3dDevice,
		static_cast<uint8_t *>(fontResources[2].data),
		fontResources[2].size);

	engineParams.d3dDeviceContext = d3dDeviceContext;

	return new FontEngine(engineParams);
}

void FontEngine::DestroyFontEngine(FontEngine *engine)
{
	if (engine == 0)
		return;

	DynamicVertexBuffers::DestroyDynamicVertexBuffers(engine->vertexBuffers_);
	VertexShader::DestroyVertexShader(engine->vertexShader_);
	PixelShader::DestroyPixelShader(engine->pixelShader_);
	MatrixBuffer::DestroyMatrixBuffer(engine->modelViewProjection_);

	delete engine;
}

void FontEngine::BeginFrame()
{
}

void FontEngine::EndFrame()
{
}

int FontEngine::DrawText(const std::string &text,
	int x,
	int y,
	uint32_t colour)
{
	return DrawText(text, x, y, colour, FONT_TYPE_DEFAULT);
}

int FontEngine::DrawText(const std::string &text,
	int x,
	int y,
	uint32_t colour,
	FontType type)
{
	int lineSpacing = 0;

	FontTypeMap::const_iterator fontTypeIt = fonts_.find(type);
	if (fontTypeIt != fonts_.end())
	{
		// Font
		Font font = fontTypeIt->second;

		// Build up the glyph vertices
		SpriteFontRenderer renderer(colour);
		font.sprite->DrawString(&renderer, text.c_str(), XMVectorSet(static_cast<float>(x), static_cast<float>(y), 0.0f, 0.0f), XMVectorZero());

		// Copy the glyph vertices
		DynamicVertexBuffers::VertexRange copiedRange;
		bool copiedVerts = renderer.Flush(vertexBuffers_,
			d3dDeviceContext_,
			&copiedRange);
		if (copiedVerts == false)
		{
			return lineSpacing;
		}

		// Set up our shaders
		vertexShader_->VSSetShader(d3dDeviceContext_);
		pixelShader_->PSSetShader(d3dDeviceContext_);

		// Flush constant buffers
		modelViewProjection_->VSSetConstantBuffers(d3dDeviceContext_,
			XMMatrixIdentity(),
			XMMatrixIdentity(),
			XMLoadFloat4x4(&projectionMatrix_));

		// Font texture
		d3dDeviceContext_->PSSetShaderResources(0, 1, &font.texture);
		d3dDeviceContext_->PSSetSamplers(0, 1, &textureSampler_);

		// Issue draw command
		d3dDeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		vertexBuffers_->IASetVertexBuffer(d3dDeviceContext_);
		d3dDeviceContext_->Draw(copiedRange.end - copiedRange.begin, copiedRange.begin);

		// Next line
		lineSpacing = static_cast<int>(font.sprite->GetLineSpacing());
	}

	return lineSpacing;
}

int FontEngine::CalculateTextWidth(const std::string &text) const
{
	return CalculateTextWidth(text, FONT_TYPE_DEFAULT);
}

int FontEngine::CalculateTextWidth(const std::string &text, FontType type) const
{
	int textWidth = 0;

	FontTypeMap::const_iterator fontTypeIt = fonts_.find(type);
	if (fontTypeIt != fonts_.end())
	{
		textWidth = static_cast<int>(XMVectorGetX(fontTypeIt->second.sprite->MeasureString(text.c_str())));
	}

	return textWidth;
}

FontEngine::Font FontEngine::CreateFont(ID3D11Device *d3dDevice, uint8_t *data, uint32_t size)
{
	Font ret;
	ret.sprite = new DirectX::SpriteFont(d3dDevice, data, size);
	ret.sprite->GetSpriteSheet(&ret.texture);
	return ret;
}
