#ifndef IMMEDIATEMODE_H_INCLUDED
#define IMMEDIATEMODE_H_INCLUDED

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class ResourceLoader;
class DynamicVertexBuffers;
class VertexShader;
class PixelShader;
class MatrixBuffer;
struct ImmediateModeVertex;

class ImmediateMode
{
public:

	static ImmediateMode *CreateImmediateMode(ResourceLoader *resources,
		ID3D11Device *d3dDevice,
		ID3D11DeviceContext *d3dDeviceContext);
	static void DestroyImmediateMode(ImmediateMode *mode);

	void BeginFrame();
	void EndFrame();

	void SetModelMatrix(XMMATRIX modelMatrix);
	void SetViewMatrix(XMMATRIX viewMatrix);
	void SetProjectionMatrix(XMMATRIX projectionMatrix);

	void Draw(D3D11_PRIMITIVE_TOPOLOGY primType,
		const ImmediateModeVertex *vertices,
		unsigned int vertexCount);

private:

	ImmediateMode(ID3D11DeviceContext *d3dDeviceContext,
		DynamicVertexBuffers *vertexBuffers,
		VertexShader *vertexShader,
		PixelShader *pixelShader,
		MatrixBuffer *modelViewProjection);
	~ImmediateMode();

	ID3D11DeviceContext *d3dDeviceContext_;

	DynamicVertexBuffers *vertexBuffers_;
	VertexShader *vertexShader_;
	PixelShader *pixelShader_;
	MatrixBuffer *modelViewProjection_;

	XMFLOAT4X4 modelMatrix_;
	XMFLOAT4X4 viewMatrix_;
	XMFLOAT4X4 projectionMatrix_;
};

#endif // IMMEDIATEMODER_H_INCLUDED
