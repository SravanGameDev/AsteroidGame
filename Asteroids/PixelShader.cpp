#include "PixelShader.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(ID3D11PixelShader *shader) :
	shader_(shader)
{
}

PixelShader::~PixelShader()
{
}

PixelShader *PixelShader::CreatePixelShader(void *shaderSource,
	size_t sourceSize,
	ID3D11Device *d3dDevice)
{
	ID3DBlob *shaderBuffer;
	HRESULT compile = D3DCompile(shaderSource,
		sourceSize,
		NULL, // Source name
		NULL, // Defines
		NULL, // Include handler
		"main", // Entry point
		"ps_5_0", // Target
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS,
		0, // Flags2
		&shaderBuffer,
		NULL);

	if (FAILED(compile))
	{
		return 0;
	}

	ID3D11PixelShader *shader;
	HRESULT createShader = d3dDevice->CreatePixelShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		&shader);
	shaderBuffer->Release();

	if (FAILED(createShader))
	{
		return 0;
	}

	return new PixelShader(shader);
}

void PixelShader::DestroyPixelShader(PixelShader *shader)
{
	if (shader == 0)
		return;

	if (shader->shader_)
		shader->shader_->Release();

	delete shader;
}

void PixelShader::PSSetShader(ID3D11DeviceContext *d3dDeviceContext) const
{
	d3dDeviceContext->PSSetShader(shader_, NULL, 0);
}
