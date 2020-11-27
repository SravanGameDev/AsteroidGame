#ifndef MATRIXBUFFER_H_INCLUDED
#define MATRIXBUFFER_H_INCLUDED

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class MatrixBuffer
{
public:

	static MatrixBuffer *CreateMatrixBuffer(ID3D11Device *d3dDevice);
	static void DestroyMatrixBuffer(MatrixBuffer *buffer);

	void VSSetConstantBuffers(ID3D11DeviceContext *d3dDeviceContext,
		XMMATRIX model,
		XMMATRIX view,
		XMMATRIX projection) const;

private:
	MatrixBuffer(ID3D11Buffer *buffer);
	~MatrixBuffer();

	MatrixBuffer(const MatrixBuffer &);
	void operator=(const MatrixBuffer &);

	ID3D11Buffer *buffer_;
};

#endif // MATRIXBUFFER_H_INCLUDED

