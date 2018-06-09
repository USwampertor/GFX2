#pragma once
#include <d3d11.h>
#include <vector>
#include "Mesh.h"
#include "VertexType.h"
class Device
{
public:
	Device()
	{

	}
	~Device()
	{

	}
	int width;
	int height;
	HRESULT InitializeDevice(HWND g_hWnd)
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

		//Creating DepthStencil texture descriptor (checks the pixels if are usable or not)

		//Creating Depth Stencil View Descriptor
		return hr;
	}
	void Render(ID3D11RenderTargetView* pRenderTargetView,std::vector<Mesh> meshvector)
	{
		float color[4] = {1.0f,0.0f,0.0f,1.0f };
		m_pImmediateContext->ClearRenderTargetView(pRenderTargetView, color);
		meshvector[0].Render(m_pImmediateContext);
		m_pSwapChain->Present(DXGI_SWAP_EFFECT_DISCARD, DXGI_PRESENT_DO_NOT_WAIT);
	}
	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;

	
};

