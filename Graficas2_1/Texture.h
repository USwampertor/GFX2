#pragma once
class Texture
{
public:
	Texture();
	~Texture();
	HRESULT CreateRenderTargetView(ID3D11Device* m_pd3dDevice, IDXGISwapChain* m_pSwapChain);
	HRESULT CreateDSTDescriptor(ID3D11Device* pd3dDevice, D3D11_TEXTURE2D_DESC& descDepth, int width, int height);
	HRESULT CreateDSVDescriptor(ID3D11Device* pd3dDevice, D3D11_TEXTURE2D_DESC& descDepth, ID3D11DeviceContext* pImmediateContext);
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11Texture2D* m_pDepthStencil = nullptr;
	ID3D11DepthStencilView* m_DepthStencilView = nullptr;
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
HRESULT Texture::CreateDSTDescriptor(ID3D11Device* pd3dDevice, D3D11_TEXTURE2D_DESC& descDepth, int width, int height)
{
	HRESULT hr = S_OK;
	//D3D11_TEXTURE2D_DESC descDepth;
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
	return hr;
}
HRESULT Texture::CreateDSVDescriptor(ID3D11Device* pd3dDevice, D3D11_TEXTURE2D_DESC& descDepth, ID3D11DeviceContext* pImmediateContext)
{
	HRESULT hr = S_OK;
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