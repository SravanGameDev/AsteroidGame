#include "SpriteFontRenderer.h"

SpriteFontRenderer::SpriteFontRenderer(uint32_t diffuse) :
	diffuse_(diffuse)
{
}

void SpriteFontRenderer::DrawGlyph(XMVECTOR position, const RECT *uvs)
{
	float left = XMVectorGetX(position);
	float right = left + (uvs->right - uvs->left);
	float top = XMVectorGetY(position);
	float bottom = top + (uvs->bottom - uvs->top);

	SpriteFontVertex topLeft;
	topLeft.x = left;
	topLeft.y = top;
	topLeft.u = static_cast<float>(uvs->left);
	topLeft.v = static_cast<float>(uvs->top);
	topLeft.diffuse = diffuse_;

	SpriteFontVertex topRight;
	topRight.x = right;
	topRight.y = top;
	topRight.u = static_cast<float>(uvs->right);
	topRight.v = static_cast<float>(uvs->top);
	topRight.diffuse = diffuse_;

	SpriteFontVertex bottomLeft;
	bottomLeft.x = left;
	bottomLeft.y = bottom;
	bottomLeft.u = static_cast<float>(uvs->left);
	bottomLeft.v = static_cast<float>(uvs->bottom);
	bottomLeft.diffuse = diffuse_;

	SpriteFontVertex bottomRight;
	bottomRight.x = right;
	bottomRight.y = bottom;
	bottomRight.u = static_cast<float>(uvs->right);
	bottomRight.v = static_cast<float>(uvs->bottom);
	bottomRight.diffuse = diffuse_;

	// Quad rendered as two triangles:
	// 0-1  5
	// |/  /|
	// 2  4-3

	vertices_.push_back(topLeft);
	vertices_.push_back(topRight);
	vertices_.push_back(bottomLeft);

	vertices_.push_back(bottomRight);
	vertices_.push_back(bottomLeft);
	vertices_.push_back(topRight);
}

bool SpriteFontRenderer::Flush(DynamicVertexBuffers *verts,
	ID3D11DeviceContext *d3dDeviceContext,
	DynamicVertexBuffers::VertexRange *copiedRange)
{
	if (vertices_.empty())
		return false;

	bool copied = verts->CopyVertexData<SpriteFontVertex>(&vertices_[0],
		static_cast<unsigned int>(vertices_.size()),
		d3dDeviceContext,
		copiedRange);

	vertices_.clear();

	return copied;
}
