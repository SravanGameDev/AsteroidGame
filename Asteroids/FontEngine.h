#ifndef FONTENGINE_H_INCLUDED
#define FONTENGINE_H_INCLUDED

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <map>

using namespace DirectX;

namespace DirectX
{
	class SpriteFont;
}

class ResourceLoader;
class DynamicVertexBuffers;
class VertexShader;
class PixelShader;
class MatrixBuffer;

class FontEngine
{
public:

	static FontEngine *CreateFontEngine(ResourceLoader *resources,
		ID3D11Device *d3dDevice,
		ID3D11DeviceContext *d3dDeviceContext);
	static void DestroyFontEngine(FontEngine *engine);

	enum FontType
	{
		FONT_TYPE_LARGE,
		FONT_TYPE_MEDIUM,
		FONT_TYPE_SMALL,

		FONT_TYPE_DEFAULT = FONT_TYPE_MEDIUM
	};

	void BeginFrame();
	void EndFrame();

	int DrawText(const std::string &text,
		int x,
		int y,
		uint32_t colour);

	int DrawText(const std::string &text,
		int x,
		int y,
		uint32_t colour,
		FontType type);

	int CalculateTextWidth(const std::string &text) const;
	int CalculateTextWidth(const std::string &text, FontType type) const;

private:

	struct Font
	{
		DirectX::SpriteFont *sprite;
		ID3D11ShaderResourceView *texture;
	};

	typedef std::map<FontType, Font> FontTypeMap;

	struct InitialisationParams
	{
		ID3D11DeviceContext *d3dDeviceContext;
		FontTypeMap fonts;
		DynamicVertexBuffers *vertexBuffers;
		VertexShader *vertexShader;
		PixelShader *pixelShader;
		MatrixBuffer *modelViewProjection;
		ID3D11SamplerState *textureSampler;
	};

	FontEngine(const InitialisationParams &initParams);
	~FontEngine();

	static Font CreateFont(ID3D11Device *d3dDevice,
		uint8_t *data,
		uint32_t size);

	ID3D11DeviceContext *d3dDeviceContext_;

	DynamicVertexBuffers *vertexBuffers_;
	VertexShader *vertexShader_;
	PixelShader *pixelShader_;
	MatrixBuffer *modelViewProjection_;
	ID3D11SamplerState *textureSampler_;

	FontTypeMap fonts_;

	XMFLOAT4X4 projectionMatrix_;
};

#endif // FONTENGINE_H_INCLUDED
