#pragma once
#include <d3d11.h>
#include <vector>
#include "Texture2D.h"
class Device
{
public:
	int
		width,
		height;
	ID3D11Device*			m_pd3dDevice;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11DeviceContext*	m_pImmediateContext;
	
	Device();
	~Device();
	HRESULT InitDevice(HWND g_hWnd, Texture2D texture);
	HRESULT RTV(HWND g_hWnd, Texture2D texture);
	HRESULT CDS(Texture2D texture);
};

Device::Device()
{
	m_pd3dDevice = nullptr;
	m_pSwapChain = nullptr;
	m_pImmediateContext = nullptr;
	
}


Device::~Device()
{
}
HRESULT Device::InitDevice(HWND g_hWnd,Texture2D texture)
{
	HRESULT hr = S_OK;
	//Obtains the size of window
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	unsigned int createDeviceFlags = 0;
#if defined (DEBUG)
	createDeviceFlags | = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	//Creating driver type vector
	std::vector<D3D_DRIVER_TYPE>driverTypes;
	driverTypes.push_back(D3D_DRIVER_TYPE_HARDWARE);
	driverTypes.push_back(D3D_DRIVER_TYPE_WARP);
	driverTypes.push_back(D3D_DRIVER_TYPE_REFERENCE);
	//We are using the default levels in the D3D library
	std::vector<D3D_FEATURE_LEVEL> featureLevels;
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
			&m_pSwapChain,
			&m_pd3dDevice,
			&selectedFL,
			&m_pImmediateContext);
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
	texture.SetWidthHeight(width, height);
	if(FAILED(texture.CreateRenderTargetView(m_pd3dDevice, m_pSwapChain))) return hr;
	if(FAILED(texture.CreateDepthStencylTexture(m_pd3dDevice)))return hr;
	if(FAILED(texture.CreteDepthStencylDescriptor(m_pd3dDevice, m_pImmediateContext)))return hr;
	return hr;
}
/*HRESULT Device::RTV(HWND g_hWnd, Texture2D texture)
{
	HRESULT hr = S_OK;

	//Create Render Target View
	ID3D11Texture2D* pbackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pbackBuffer));
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pd3dDevice->CreateRenderTargetView(pbackBuffer, nullptr, &texture.m_pRenderTargetView);
	pbackBuffer->Release();
	if (FAILED(hr))
	{
		return hr;
	}
	
	//Create Depth S View
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

	hr = m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &texture.m_pDepthStencil);
	return hr;
}
HRESULT Device::CDS(Texture2D texture)
{
	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	memset(&descDSV, 0, sizeof(descDSV));

	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = m_pd3dDevice->CreateDepthStencilView(texture.m_pDepthStencil, &descDSV, &texture.m_pDepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}

	m_pImmediateContext->OMSetRenderTargets(1, &texture.m_pRenderTargetView, texture.m_pDepthStencilView);

	return hr;
}*/