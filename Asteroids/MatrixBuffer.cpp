#include "MatrixBuffer.h"

MatrixBuffer::MatrixBuffer(ID3D11Buffer *buffer) :
	buffer_(buffer)
{

}

MatrixBuffer::~MatrixBuffer()
{

}

MatrixBuffer *MatrixBuffer::CreateMatrixBuffer(ID3D11Device *d3dDevice)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = sizeof(XMMATRIX) * 3;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	ID3D11Buffer *buffer;
	HRESULT createBuffer = d3dDevice->CreateBuffer(&desc, NULL, &buffer);
	if (FAILED(createBuffer))
	{
		return 0;
	}

	return new MatrixBuffer(buffer);
}

void MatrixBuffer::DestroyMatrixBuffer(MatrixBuffer *buffer)
{
	if (buffer == 0)
		return;

	if (buffer->buffer_)
		buffer->buffer_->Release();

	delete buffer;
}

void MatrixBuffer::VSSetConstantBuffers(ID3D11DeviceContext *d3dDeviceContext, XMMATRIX model, XMMATRIX view, XMMATRIX projection) const
{
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hr = d3dDeviceContext->Map(buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (FAILED(hr))
	{
		return;
	}

	XMMATRIX *matrices = static_cast<XMMATRIX *>(resource.pData);
	matrices[0] = XMMatrixTranspose(model);
	matrices[1] = XMMatrixTranspose(view);
	matrices[2] = XMMatrixTranspose(projection);

	d3dDeviceContext->Unmap(buffer_, 0);

	d3dDeviceContext->VSSetConstantBuffers(0,
		1,
		&buffer_);
}
