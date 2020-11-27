#ifndef SPRITEFONTRENDERER_H_INCLUDED
#define SPRITEFONTRENDERER_H_INCLUDED

#include "SpriteFontVertex.h"
#include "DynamicVertexBuffers.h"
#include <DirectXMath.h>
#include <SpriteFont.h>
#include <vector>

using namespace DirectX;

class Graphics;

class SpriteFontRenderer : public DirectX::ISpriteFontRenderer
{
public:

	SpriteFontRenderer(uint32_t diffuse);

	virtual void DrawGlyph(XMVECTOR position, const RECT *uvs);

	bool Flush(DynamicVertexBuffers *verts,
		ID3D11DeviceContext *d3dDeviceContext,
		DynamicVertexBuffers::VertexRange *copiedRange);

private:
	typedef std::vector<SpriteFontVertex> FontVertexVector;

	FontVertexVector vertices_;
	uint32_t diffuse_;
};

#endif // SPRITEFONTRENDERER_H_INCLUDED
