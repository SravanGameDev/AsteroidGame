#ifndef PIXELSHADER_H_INCLUDED
#define PIXELSHADER_H_INCLUDED

#include <d3d11.h>

class PixelShader
{
public:

	static PixelShader *CreatePixelShader(void *shaderSource,
		size_t sourceSize,
		ID3D11Device *d3dDevice);
	static void DestroyPixelShader(PixelShader *shader);

	void PSSetShader(ID3D11DeviceContext *d3dDeviceContext) const;

private:
	PixelShader(ID3D11PixelShader *shader);
	~PixelShader();

	PixelShader(const PixelShader &);
	void operator=(const PixelShader &);

	ID3D11PixelShader *shader_;
};

#endif // PIXELSHADER_H_INCLUDED
