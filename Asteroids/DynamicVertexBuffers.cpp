#include "DynamicVertexBuffers.h"

DynamicVertexBuffers::DynamicVertexBuffers(const BufferVector &buffers,
	unsigned int vertexStride,
	unsigned int bufferCapacity) :
	vertexStride_(vertexStride),
	buffers_(buffers),
	bufferCapacity_(bufferCapacity),
	currentBuffer_(0),
	currentBufferSize_(0)
{
}

DynamicVertexBuffers::~DynamicVertexBuffers()
{
}

DynamicVertexBuffers *DynamicVertexBuffers::CreateDynamicVertexBuffers(unsigned int maximumVertexCount,
	unsigned int vertexStride,
	unsigned int bufferCount,
	ID3D11Device *d3dDevice)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.ByteWidth = vertexStride * maximumVertexCount;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = vertexStride;

	BufferVector vertexBuffers;
	vertexBuffers.reserve(bufferCount);

	for (unsigned int i = 0; i < bufferCount; i++)
	{
		ID3D11Buffer *vertexBuffer;
		HRESULT createBuffer = d3dDevice->CreateBuffer(&bufferDesc,
			NULL,
			&vertexBuffer);
		if (FAILED(createBuffer))
		{
			ReleaseVertexBuffers(&vertexBuffers);
			return 0;
		}

		vertexBuffers.push_back(vertexBuffer);
	}

	return new DynamicVertexBuffers(vertexBuffers, vertexStride, maximumVertexCount);
}

void DynamicVertexBuffers::DestroyDynamicVertexBuffers(DynamicVertexBuffers *renderer)
{
	if (renderer == 0)
		return;

	ReleaseVertexBuffers(&renderer->buffers_);

	delete renderer;
}

void DynamicVertexBuffers::BeginFrame()
{
	currentBuffer_ = (currentBuffer_ + 1) % buffers_.size();
	currentBufferSize_ = 0;
}

bool DynamicVertexBuffers::CopyVertexData(const void *vertices,
	unsigned int vertexSize,
	unsigned int vertexCount,
	ID3D11DeviceContext *d3dDeviceContext,
	VertexRange *copiedRange)
{
	// Do we have enough space left in the vertex buffer?
	if ((currentBufferSize_ + vertexCount) > bufferCapacity_)
	{
		return false;
	}

	// Copy in the data
	D3D11_MAP mapType = (currentBufferSize_ == 0) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE;
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT mapResource = d3dDeviceContext->Map(buffers_[currentBuffer_],
		0,
		mapType,
		0,
		&resource);
	if (FAILED(mapResource))
	{
		return false;
	}

	void *destination = static_cast<uint8_t *>(resource.pData) + currentBufferSize_ * vertexSize;
	memcpy(destination, vertices, vertexCount * vertexSize);

	d3dDeviceContext->Unmap(buffers_[currentBuffer_], 0);

	// Details of copied area
	copiedRange->begin = currentBufferSize_;
	copiedRange->end = currentBufferSize_ + vertexCount;

	// Update space used
	currentBufferSize_ += vertexCount;

	return true;
}

void DynamicVertexBuffers::IASetVertexBuffer(ID3D11DeviceContext *d3dDeviceContext) const
{
	unsigned int stride = vertexStride_;
	unsigned int offset = 0;
	d3dDeviceContext->IASetVertexBuffers(
		0, // StartSlot
		1, // NumBuffers
		&buffers_[currentBuffer_],
		&stride,
		&offset);
}

void DynamicVertexBuffers::EndFrame()
{
	// Shouldn't need to do anything; maybe some usage stats would be handy
}

void DynamicVertexBuffers::ReleaseVertexBuffers(BufferVector *buffers)
{
	for (BufferVector::iterator bufferIt = buffers->begin();
		bufferIt != buffers->end();
		++bufferIt)
	{
		(*bufferIt)->Release();
	}
}
