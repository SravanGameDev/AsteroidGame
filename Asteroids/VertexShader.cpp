#include "VertexShader.h"
#include <d3dcompiler.h>

VertexShader::VertexShader(ID3D11VertexShader *shader,
	ID3D11InputLayout *layout) :
	shader_(shader),
	layout_(layout)
{
}

VertexShader::~VertexShader()
{
}

VertexShader *VertexShader::CreateVertexShader(void *shaderSource,
	size_t sourceSize,
	ID3D11Device *d3dDevice,
	const std::vector<D3D11_INPUT_ELEMENT_DESC> &vertexLayout)
{
	ID3DBlob *shaderBuffer;
	HRESULT compile = D3DCompile(shaderSource,
		sourceSize,
		NULL, // Source name
		NULL, // Defines
		NULL, // Include handler
		"main", // Entry point
		"vs_5_0", // Target
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS,
		0, // Flags2
		&shaderBuffer,
		NULL);

	if (FAILED(compile))
	{
		return 0;
	}

	ID3D11VertexShader *shader;
	HRESULT createShader = d3dDevice->CreateVertexShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		&shader);

	if (FAILED(createShader))
	{
		shaderBuffer->Release();
		return 0;
	}

	ID3D11InputLayout *layout;
	HRESULT createLayout = d3dDevice->CreateInputLayout(
		&vertexLayout[0],
		static_cast<UINT>(vertexLayout.size()),
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		&layout);

	shaderBuffer->Release();

	if (FAILED(createLayout))
	{
		shader->Release();
		return 0;
	}

	return new VertexShader(shader, layout);
}

void VertexShader::DestroyVertexShader(VertexShader *shader)
{
	if (shader == 0)
		return;

	if (shader->layout_)
		shader->layout_->Release();

	if (shader->shader_)
		shader->shader_->Release();

	delete shader;
}

void VertexShader::VSSetShader(ID3D11DeviceContext *d3dDeviceContext) const
{
	d3dDeviceContext->IASetInputLayout(layout_);
	d3dDeviceContext->VSSetShader(shader_, NULL, 0);
}
