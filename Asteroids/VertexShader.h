#ifndef VERTEXSHADER_H_INCLUDED
#define VERTEXSHADER_H_INCLUDED

#include <d3d11.h>
#include <vector>

class VertexShader
{
public:

	static VertexShader *CreateVertexShader(void *shaderSource,
		size_t sourceSize,
		ID3D11Device *d3dDevice,
		const std::vector<D3D11_INPUT_ELEMENT_DESC> &vertexLayout);
	static void DestroyVertexShader(VertexShader *shader);

	void VSSetShader(ID3D11DeviceContext *d3dDeviceContext) const;

private:
	VertexShader(ID3D11VertexShader *shader,
		ID3D11InputLayout *layout);
	~VertexShader();

	VertexShader(const VertexShader &);
	void operator=(const VertexShader &);

	ID3D11VertexShader *shader_;
	ID3D11InputLayout *layout_;
};

#endif // VERTEXSHADER_H_INCLUDED
