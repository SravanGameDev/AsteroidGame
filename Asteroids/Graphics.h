#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "AssetManager.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class ResourceLoader;
class ImmediateMode;
class FontEngine;

class Graphics
{
public:
	static Graphics *CreateDevice(HWND window, ResourceLoader *binaryResources);
	static void DestroyDevice(Graphics *device);

	void BeginFrame();
	void EndFrame();

	void ClearFrame(float r, float g, float b, float a);

	ImmediateMode *GetImmediateMode() const;
	FontEngine *GetFontEngine() const;

private:

	struct InitialisationParams
	{
		IDXGISwapChain * dxgiSwapChain;
		ID3D11Device *d3dDevice;
		ID3D11DeviceContext *d3dDeviceContext;
		ID3D11RenderTargetView *d3dRenderTargetView;
	};

	Graphics(const InitialisationParams &initParams);
	~Graphics();

	Graphics(const Graphics &);
	void operator=(const Graphics &);

	bool CreateResources(ResourceLoader *binaryResources);
	bool CreateSpriteFontResources(ResourceLoader *binaryResources);
	void DestroyResources();
	void DestroySpriteFontResources();

	IDXGISwapChain *dxgiSwapChain_;

	ID3D11Device *d3dDevice_;
	ID3D11DeviceContext *d3dDeviceContext_;
	ID3D11RenderTargetView *d3dRenderTargetView_;

	D3D11_VIEWPORT defaultViewport_;

	ImmediateMode *immediateMode_;
	FontEngine *fontEngine_;
};

#endif GRAPHICS_H_INCLUDED
