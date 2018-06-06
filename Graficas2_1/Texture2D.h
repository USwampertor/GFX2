#pragma once
#include <d3d11.h>

enum TextureCreationFlags
{
	NOT = 0,
	SRV = 1,
	RTV = 1 << 1,
	DSV = 1 << 2,
	SSRT = SRV | RTV,
	SSDT = SRV | DSV
};

class Texture2D
{
public:
	int
		m_width, m_height;
	ID3D11Texture2D* m_pBackBuffer;
	ID3D11Texture2D * m_pDepthStencil;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RenderTargetView* m_pRenderTargetView;
	D3D11_TEXTURE2D_DESC descDepth;
	Texture2D();
	Texture2D(int w, int h, int flag);
	~Texture2D();
	void SetWidthHeight(float width, float height);
	HRESULT CreateRenderTargetView(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain);
	HRESULT CreateDepthStencylTexture(ID3D11Device* pd3dDevice);
	HRESULT CreteDepthStencylDescriptor(ID3D11Device* pd3dDevice, ID3D11DeviceContext*	pImmediateContext);

};

void Texture2D::SetWidthHeight(float width, float height)
{
	m_width = width;
	m_height = height;
}
Texture2D::Texture2D()
{
	m_pBackBuffer = nullptr;
	m_pDepthStencil = nullptr;
	m_pDepthStencilView = nullptr;
	m_pRenderTargetView = nullptr;
}
Texture2D::Texture2D(int w, int h, int flag)
{
	m_pDepthStencil = nullptr;
	m_pDepthStencilView = nullptr;
	m_pRenderTargetView = nullptr;
	m_width = w;
	m_height = h;
}

Texture2D::~Texture2D()
{
}
HRESULT Texture2D::CreateRenderTargetView(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain)
{
	
	HRESULT hr = S_OK;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&m_pBackBuffer));
	if (FAILED(hr))
	{
		return hr;
	}
	hr = pd3dDevice->CreateRenderTargetView(m_pBackBuffer, nullptr, &m_pRenderTargetView);
	m_pBackBuffer->Release();
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}
HRESULT Texture2D::CreateDepthStencylTexture(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;
	//Creating DepthStencil texture descriptor (checks the pixels if are usable or not)
	memset(&descDepth, 0, sizeof(descDepth));
	descDepth.Width = m_width;
	descDepth.Height = m_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;

}
HRESULT Texture2D::CreteDepthStencylDescriptor(ID3D11Device* pd3dDevice, ID3D11DeviceContext*	pImmediateContext)
{
	HRESULT hr = S_OK;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	memset(&descDSV, 0, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}

	pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	return hr;
}