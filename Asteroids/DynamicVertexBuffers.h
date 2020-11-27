#ifndef DYNAMICVERTEXBUFFERS_H_INCLUDED
#define DYNAMICVERTEXBUFFERS_H_INCLUDED

#include "ImmediateModeVertex.h"
#include <d3d11.h>
#include <vector>

class DynamicVertexBuffers
{
public:

	struct VertexRange
	{
		unsigned int begin;
		unsigned int end;
	};

	static DynamicVertexBuffers *CreateDynamicVertexBuffers(unsigned int maximumVertexCount,
		unsigned int vertexStride,
		unsigned int bufferCount,
		ID3D11Device *d3dDevice);
	static void DestroyDynamicVertexBuffers(DynamicVertexBuffers *renderer);

	void BeginFrame();
	bool CopyVertexData(const void *vertices,
		unsigned int vertexSize,
		unsigned int vertexCount,
		ID3D11DeviceContext *d3dDeviceContext,
		VertexRange *copiedRange);
	void IASetVertexBuffer(ID3D11DeviceContext *d3dDeviceContext) const;
	void EndFrame();

	template <typename VERTEX_TYPE>
	static DynamicVertexBuffers *CreateDynamicVertexBuffers(unsigned int maximumVertexCount,
		unsigned int bufferCount,
		ID3D11Device *d3dDevice)
	{
		return CreateDynamicVertexBuffers(maximumVertexCount,
			sizeof(VERTEX_TYPE),
			bufferCount,
			d3dDevice);
	}

	template <typename VERTEX_TYPE>
	bool CopyVertexData(const VERTEX_TYPE *vertices,
		unsigned int vertexCount,
		ID3D11DeviceContext *d3dDeviceContext,
		VertexRange *copiedRange)
	{
		return CopyVertexData(vertices,
			sizeof(VERTEX_TYPE),
			vertexCount,
			d3dDeviceContext,
			copiedRange);
	}

private:
	typedef std::vector<ID3D11Buffer *> BufferVector;

	DynamicVertexBuffers(const BufferVector &buffers,
		unsigned int vertexStride,
		unsigned int bufferCapacity);
	~DynamicVertexBuffers();

	DynamicVertexBuffers(const DynamicVertexBuffers &);
	void operator=(const DynamicVertexBuffers &);

	static void ReleaseVertexBuffers(BufferVector *buffers);

	unsigned int vertexStride_;

	BufferVector buffers_;
	unsigned int bufferCapacity_;

	unsigned int currentBuffer_;
	unsigned int currentBufferSize_;

};

#endif // DYNAMICVERTEXBUFFERS_H_INCLUDED
