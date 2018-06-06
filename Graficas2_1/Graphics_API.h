#pragma once
#include <d3d11.h>
#include <vector>
#include "Device.h"
#include "Texture2D.h"
#include "ViewPort.h"
using std::vector;
class Graphics_API
{
public:
	ViewPort m_viewPort;
	Device m_device;
	Texture2D m_texture2D;
	Graphics_API();
	~Graphics_API();
	HRESULT InitDevice(HWND& g_hWnd);
	void Render();
};
Graphics_API::Graphics_API()
{

}
Graphics_API::~Graphics_API()
{
}
HRESULT Graphics_API::InitDevice(HWND& g_hWnd)
{
	HRESULT hr = S_OK;
	hr = m_device.InitDevice(g_hWnd,m_texture2D);
	if (FAILED(hr))
	{
		return hr;
	}
	//hr = m_device.RTV(g_hWnd,m_texture2D);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}
	m_viewPort.CreateViewPort(m_device);
	return hr;
}
void Graphics_API::Render()
{
	float color[4] = { 1.0f,0.0f,1.0f,1.0f };
	m_device.m_pImmediateContext->ClearRenderTargetView(m_texture2D.m_pRenderTargetView, color);
	m_device.m_pSwapChain->Present(DXGI_SWAP_EFFECT_DISCARD, DXGI_PRESENT_DO_NOT_WAIT);
}
