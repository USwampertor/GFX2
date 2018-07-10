#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
class Texture
{
public:
	Texture();
	~Texture();
	HRESULT Create2DTexture();
	HRESULT CreateRenderTargetView(
		ID3D11Device* m_pd3dDevice, 
		IDXGISwapChain* m_pSwapChain);
	HRESULT CreateDSTDescriptor(
		ID3D11Device* pd3dDevice, 
		ID3D11DeviceContext* pImmediateContext, 
		int width, 
		int height);
	HRESULT CreateDSVDescriptor(
		ID3D11Device* pd3dDevice, 
		ID3D11DeviceContext* pImmediateContext);
	HRESULT CreateDSS(ID3D11Device* pd3dDevice);
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11Texture2D* m_pDepthStencil = nullptr;
	ID3D11DepthStencilView* m_DepthStencilView = nullptr;
	ID3D11ShaderResourceView* m_shaderSubResource = nullptr;
	/*From the tutorial
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11RasterizerState* m_rasterState;
	HRESULT CreateRasterState(
		ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pImmediateContext, 
		D3D11_RASTERIZER_DESC& descRastr);*/
};
Texture::Texture()
{
}


Texture::~Texture()
{
}

HRESULT Texture::CreateRenderTargetView(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain)
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
	if (FAILED(hr))
	{
		return hr;
	}
	hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();
	
	return hr;
	
}
HRESULT Texture::CreateDSTDescriptor(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, int width, int height)
{
	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC descDepth;
	//This allows us to control what type of depth test Direct3D will do for each pixel.
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

	hr = pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);

	//D3D11_TEXTURE2D_DESC descDepth;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

	memset(&descDSV, 0, sizeof(descDSV));

	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_DepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}

	pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_DepthStencilView);

	return hr;
}
HRESULT Texture::CreateDSVDescriptor(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext)
{
	HRESULT hr = S_OK;
	

	return hr;
}
HRESULT Texture::CreateDSS(ID3D11Device* pd3dDevice)
{
	D3D11_TEXTURE2D_DESC descDepth;
	HRESULT hr = S_OK;
	//hr = pd3dDevice->CreateDepthStencilState(d, &m_depthStencilState);
	//if (FAILED(hr))
	//{
	return hr;
	//}
}
HRESULT Texture::Create2DTexture()
{

}
/*HRESULT Texture::CreateRasterState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, D3D11_RASTERIZER_DESC& descRastr)
{
	HRESULT hr = S_OK;
	descRastr.AntialiasedLineEnable = false;
	descRastr.CullMode = D3D11_CULL_BACK;
	descRastr.DepthBias = 0;
	descRastr.DepthBiasClamp = 0.0f;
	descRastr.DepthClipEnable = true;
	descRastr.FillMode = D3D11_FILL_SOLID;
	descRastr.FrontCounterClockwise = false;
	descRastr.MultisampleEnable = false;
	descRastr.ScissorEnable = false;
	descRastr.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = pd3dDevice->CreateRasterizerState(&descRastr, &m_rasterState);
	if (FAILED(hr))
	{
		return false;
	}

	// Now set the rasterizer state.
	pImmediateContext->RSSetState(m_rasterState);
	return hr;
}*/