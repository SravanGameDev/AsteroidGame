#include "ImmediateMode.h"
#include "DynamicVertexBuffers.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "MatrixBuffer.h"
#include "ResourceLoader.h"
#include "resource.h"

ImmediateMode::ImmediateMode(ID3D11DeviceContext *d3dDeviceContext,
	DynamicVertexBuffers *vertexBuffers,
	VertexShader *vertexShader,
	PixelShader *pixelShader,
	MatrixBuffer *modelViewProjection) :
	d3dDeviceContext_(d3dDeviceContext),
	vertexBuffers_(vertexBuffers),
	vertexShader_(vertexShader),
	pixelShader_(pixelShader),
	modelViewProjection_(modelViewProjection)
{
	XMStoreFloat4x4(&modelMatrix_, XMMatrixIdentity());
	XMStoreFloat4x4(&viewMatrix_, XMMatrixIdentity());
	XMStoreFloat4x4(&projectionMatrix_, XMMatrixIdentity());
}

ImmediateMode::~ImmediateMode()
{
}

ImmediateMode *ImmediateMode::CreateImmediateMode(ResourceLoader *resources,
	ID3D11Device *d3dDevice,
	ID3D11DeviceContext *d3dDeviceContext)
{
	const unsigned int MAX_IMMEDIATE_MODE_VERTICES = 1 * 1024 * 1024;

	ResourceLoader::Resource vertexShaderResource;
	resources->LoadResource(IDR_VERTEX_SHADER_FVF_XYZ_DIFFUSE, &vertexShaderResource);

	ResourceLoader::Resource pixelShaderResource;
	resources->LoadResource(IDR_PIXEL_SHADER_FVF_XYZ_DIFFUSE, &pixelShaderResource);

	DynamicVertexBuffers *vertexBuffers = DynamicVertexBuffers::CreateDynamicVertexBuffers<ImmediateModeVertex>(
		MAX_IMMEDIATE_MODE_VERTICES,
		2,
		d3dDevice);
	MatrixBuffer *modelViewProjection = MatrixBuffer::CreateMatrixBuffer(d3dDevice);

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.resize(2);

	vertexLayout[0].SemanticName = "POSITION";
	vertexLayout[0].SemanticIndex = 0;
	vertexLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexLayout[0].InputSlot = 0;
	vertexLayout[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[0].InstanceDataStepRate = 0;

	vertexLayout[1].SemanticName = "COLOR";
	vertexLayout[1].SemanticIndex = 0;
	vertexLayout[1].Format = DXGI_FORMAT_R8G8B8A8_UINT;
	vertexLayout[1].InputSlot = 0;
	vertexLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[1].InstanceDataStepRate = 0;

	VertexShader *vertexShader = VertexShader::CreateVertexShader(
		vertexShaderResource.data,
		vertexShaderResource.size,
		d3dDevice,
		vertexLayout);

	PixelShader *pixelShader = PixelShader::CreatePixelShader(
		pixelShaderResource.data,
		pixelShaderResource.size,
		d3dDevice);

	return new ImmediateMode(d3dDeviceContext,
		vertexBuffers,
		vertexShader,
		pixelShader,
		modelViewProjection);
}

void ImmediateMode::DestroyImmediateMode(ImmediateMode *mode)
{
	if (mode == 0)
		return;

	DynamicVertexBuffers::DestroyDynamicVertexBuffers(mode->vertexBuffers_);
	VertexShader::DestroyVertexShader(mode->vertexShader_);
	PixelShader::DestroyPixelShader(mode->pixelShader_);
	MatrixBuffer::DestroyMatrixBuffer(mode->modelViewProjection_);

	delete mode;
}

void ImmediateMode::BeginFrame()
{
	vertexBuffers_->BeginFrame();
}

void ImmediateMode::EndFrame()
{
	vertexBuffers_->EndFrame();
}

void ImmediateMode::SetModelMatrix(XMMATRIX modelMatrix)
{
	XMStoreFloat4x4(&modelMatrix_, modelMatrix);
}

void ImmediateMode::SetViewMatrix(XMMATRIX viewMatrix)
{
	XMStoreFloat4x4(&viewMatrix_, viewMatrix);
}

void ImmediateMode::SetProjectionMatrix(XMMATRIX projectionMatrix)
{
	XMStoreFloat4x4(&projectionMatrix_, projectionMatrix);
}

void ImmediateMode::Draw(D3D11_PRIMITIVE_TOPOLOGY primType,
	const ImmediateModeVertex *vertices,
	unsigned int vertexCount)
{
	// Copy dynamic vertex data
	DynamicVertexBuffers::VertexRange copiedRange;
	bool copiedVerts = vertexBuffers_->CopyVertexData(vertices, vertexCount, d3dDeviceContext_, &copiedRange);
	if (copiedVerts == false)
	{
		return;
	}

	// Set up our shaders
	vertexShader_->VSSetShader(d3dDeviceContext_);
	pixelShader_->PSSetShader(d3dDeviceContext_);

	// Flush constant buffers
	modelViewProjection_->VSSetConstantBuffers(d3dDeviceContext_,
		XMLoadFloat4x4(&modelMatrix_),
		XMLoadFloat4x4(&viewMatrix_),
		XMLoadFloat4x4(&projectionMatrix_));

	// Issue draw command
	d3dDeviceContext_->IASetPrimitiveTopology(primType);
	vertexBuffers_->IASetVertexBuffer(d3dDeviceContext_);
	d3dDeviceContext_->Draw(vertexCount, copiedRange.begin);
}

