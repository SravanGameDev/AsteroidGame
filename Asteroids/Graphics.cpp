#include "Graphics.h"
#include "ResourceLoader.h"
#include "ImmediateMode.h"
#include "FontEngine.h"

Graphics::Graphics(const InitialisationParams &initParams) :
	dxgiSwapChain_(initParams.dxgiSwapChain),
	d3dDevice_(initParams.d3dDevice),
	d3dDeviceContext_(initParams.d3dDeviceContext),
	d3dRenderTargetView_(initParams.d3dRenderTargetView),
	immediateMode_(0),
	fontEngine_(0)
{
	defaultViewport_.TopLeftX = 0;
	defaultViewport_.TopLeftY = 0;
	defaultViewport_.Width = 800;
	defaultViewport_.Height = 600;
	defaultViewport_.MinDepth = 0.0f;
	defaultViewport_.MaxDepth = 1.0f;
}

Graphics::~Graphics()
{
}

Graphics *Graphics::CreateDevice(HWND window, ResourceLoader *binaryResources)
{
	// Create the basic D3D devices
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = 800;
	swapChainDesc.BufferDesc.Height = 600;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGISwapChain *dxgiSwapChain;
	ID3D11Device *d3dDevice;
	ID3D11DeviceContext *d3dDeviceContext;

	const D3D_FEATURE_LEVEL requiredFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	HRESULT createDevice = D3D11CreateDeviceAndSwapChain(
		NULL, // Default adapter
		D3D_DRIVER_TYPE_WARP, // Force a slow path to make sure we're working with a known reference
		//D3D_DRIVER_TYPE_HARDWARE, // Force a slow path to make sure we're working with a known reference
		NULL, // Software rasteriser unused
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		&requiredFeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&dxgiSwapChain,
		&d3dDevice,
		NULL, // We only passed one feature level, so it either succeeded or failed
		&d3dDeviceContext);

	if (FAILED(createDevice))
	{
		return 0;
	}

	ID3D11Resource *backBuffer;
	HRESULT getBackBuffer = dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer));
	if (FAILED(getBackBuffer))
	{
		d3dDeviceContext->Release();
		d3dDevice->Release();
		dxgiSwapChain->Release();
		return 0;
	}

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	ID3D11RenderTargetView *d3dRenderTargetView;
	HRESULT createRenderTargetView = d3dDevice->CreateRenderTargetView(
		backBuffer,
		&viewDesc,
		&d3dRenderTargetView);
	backBuffer->Release();

	if (FAILED(createRenderTargetView))
	{
		d3dDeviceContext->Release();
		d3dDevice->Release();
		dxgiSwapChain->Release();
		return 0;
	}

	//  Create our graphics device
	InitialisationParams gfxParams;
	gfxParams.dxgiSwapChain = dxgiSwapChain;
	gfxParams.d3dDevice = d3dDevice;
	gfxParams.d3dDeviceContext = d3dDeviceContext;
	gfxParams.d3dRenderTargetView = d3dRenderTargetView;

	Graphics *newDevice = new Graphics(gfxParams);

	bool loadDefaultResources = newDevice->CreateResources(binaryResources);
	if (loadDefaultResources == false)
	{
		DestroyDevice(newDevice);
		return 0;
	}

	return newDevice;
}

void Graphics::DestroyDevice(Graphics *device)
{
	device->DestroyResources();

	device->d3dDeviceContext_->Release();
	device->d3dDevice_->Release();
	device->dxgiSwapChain_->Release();

	delete device;
}

void Graphics::BeginFrame()
{
	static float redFlash = 0.5f;
	redFlash = fmodf(redFlash + 0.05f, 1.0f);
	ClearFrame(redFlash, 0.0f, 0.0f, 0.0f);

	d3dDeviceContext_->RSSetViewports(1, &defaultViewport_);

	immediateMode_->BeginFrame();
	fontEngine_->BeginFrame();
}

void Graphics::EndFrame()
{
	d3dDeviceContext_->ClearState();

	immediateMode_->EndFrame();
	fontEngine_->EndFrame();
	dxgiSwapChain_->Present(1, 0);
}

void Graphics::ClearFrame(float r, float g, float b, float a)
{
	const float rgba[4] = { r, g, b, a };
	d3dDeviceContext_->ClearRenderTargetView(
		d3dRenderTargetView_,
		rgba);

	d3dDeviceContext_->OMSetRenderTargets(1, &d3dRenderTargetView_, NULL);
}

ImmediateMode *Graphics::GetImmediateMode() const
{
	return immediateMode_;
}

FontEngine *Graphics::GetFontEngine() const
{
	return fontEngine_;
}

bool Graphics::CreateResources(ResourceLoader *binaryResources)
{
	immediateMode_ = ImmediateMode::CreateImmediateMode(binaryResources, d3dDevice_, d3dDeviceContext_);
	fontEngine_ = FontEngine::CreateFontEngine(binaryResources, d3dDevice_, d3dDeviceContext_);
	if ((immediateMode_ == 0) ||
		(fontEngine_ == 0))
	{
		DestroyResources();
		return false;
	}

	return true;
}

void Graphics::DestroyResources()
{
	FontEngine::DestroyFontEngine(fontEngine_);
	ImmediateMode::DestroyImmediateMode(immediateMode_);
}
