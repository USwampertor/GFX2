#pragma once
#include <d3d11.h>
#include <vector>
using std::vector;
class Graphics_API
{
public:
	Graphics_API();
	~Graphics_API();
	HRESULT InitDevice(HWND g_hWnd);
	void Render();
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pImmediateContext = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
	ID3D11Texture2D* g_pDepthStencil = nullptr;
	ID3D11DepthStencilView* g_DepthStencilView = nullptr;
	HRESULT SetViewPort(int topx, int topy, int w, int h, float min, float max);
};
Graphics_API::Graphics_API()
{
}
Graphics_API::~Graphics_API()
{
}
HRESULT Graphics_API::InitDevice(HWND g_hWnd)
{
	HRESULT hr = S_OK;
	//Obtains the size of window
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	unsigned int createDeviceFlags = 0;
#if defined (DEBUG)
	createDeviceFlags | = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	//Creating driver type vector
	vector<D3D_DRIVER_TYPE>driverTypes;
	driverTypes.push_back(D3D_DRIVER_TYPE_HARDWARE);
	driverTypes.push_back(D3D_DRIVER_TYPE_WARP);
	driverTypes.push_back(D3D_DRIVER_TYPE_REFERENCE);
	//We are using the default levels in the D3D library
	vector<D3D_FEATURE_LEVEL> featureLevels;
	featureLevels.push_back(D3D_FEATURE_LEVEL_11_0);
	featureLevels.push_back(D3D_FEATURE_LEVEL_10_1);
	featureLevels.push_back(D3D_FEATURE_LEVEL_10_0);

	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	D3D_FEATURE_LEVEL selectedFL;

	for (size_t driverTypeIndex = 0; driverTypeIndex < driverTypes.size(); ++driverTypeIndex)
	{
		D3D_DRIVER_TYPE& dt = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			dt,
			nullptr,
			createDeviceFlags,
			&featureLevels[0],
			static_cast<UINT>(featureLevels.size()),
			D3D11_SDK_VERSION,
			&sd,
			&g_pSwapChain,
			&g_pd3dDevice,
			&selectedFL,
			&g_pImmediateContext);
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	if (FAILED(hr))
	{
		return hr;
	}

	//Creating RenderTargetView
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
	if (FAILED(hr))
	{
		return hr;
	}
	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		return hr;
	}

	//Creating DepthStencil texture descriptor (checks the pixels if are usable or not)

	D3D11_TEXTURE2D_DESC descDepth;
	memset(&descDepth, 0, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED(hr))
	{
		return hr;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

	memset(&descDSV, 0, sizeof(descDSV));

	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_DepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_DepthStencilView);

	if (FAILED(SetViewPort(0, 1, 2, 3, 4.0f, 5.0f)))
	{
		return hr;
	}

	return hr;
}
void Graphics_API::Render()
{
	float color[4] = { 1.0f,0.0f,1.0f,1.0f };
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, color);
	g_pSwapChain->Present(DXGI_SWAP_EFFECT_DISCARD, DXGI_PRESENT_DO_NOT_WAIT);
}
HRESULT Graphics_API::SetViewPort(int topx, int topy, int w, int h, float min, float max)
{
	HRESULT hr = S_OK;
	D3D11_VIEWPORT* vp = nullptr;
	vp->TopLeftX = topx;
	vp->TopLeftY = topy;
	vp->Width = w;
	vp->Height = h;
	vp->MinDepth = min;
	vp->MaxDepth = max;
	g_pImmediateContext->RSSetViewports(1, vp);
	return hr;
}
