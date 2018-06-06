#pragma once
#include <d3d11.h>
#include "Device.h"
#include "Texture.h"
using std::vector;
class Graphics_API
{
public:
	Device m_Device;
	Texture m_texture;
	Graphics_API();
	~Graphics_API();
	HRESULT InitDevice(HWND g_hWnd);
	void Render();
	
};
Graphics_API::Graphics_API()
{
}
Graphics_API::~Graphics_API()
{
}
HRESULT Graphics_API::InitDevice(HWND g_hWnd)
{
	D3D11_TEXTURE2D_DESC DSTDesc;
	HRESULT hr = m_Device.InitializeDevice(g_hWnd);
	if (FAILED(hr))
	{
		return hr;
	}
	//CreateRenderTargetView
	m_texture.CreateRenderTargetView(m_Device.m_pd3dDevice, m_Device.m_pSwapChain);
	//CreateDepthStencilTexture descriptor
	m_texture.CreateDSTDescriptor(m_Device.m_pd3dDevice, DSTDesc, m_Device.width, m_Device.height);
	//CreateDepthStencilView descriptor
	m_texture.CreateDSVDescriptor(m_Device.m_pd3dDevice, DSTDesc, m_Device.m_pImmediateContext);
	//SetViewPort
	return hr;
}
void Graphics_API::Render()
{
	m_Device.Render(m_texture.m_pRenderTargetView);
}

