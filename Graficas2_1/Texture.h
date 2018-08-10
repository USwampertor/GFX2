#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
class Texture
{
private:
	int m_width, m_height;
	//stbi_uc* m_fileData;
public:
	Texture() = default;
	~Texture() = default;
	HRESULT LoadFromFile(
		const char* fileName, 
		ID3D11Device* pd3dDevice, 
		ID3D11DeviceContext* pImmediateContext);
	HRESULT CreateShaderResourceView(ID3D11Device * pd3dDevice);
	void SetShaderResourceView(ID3D11Device * pd3dDevice);
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
	
	ID3D11Texture2D* m_pd3dTexture2D = nullptr;

	ID3D11DepthStencilView* m_DepthStencilView = nullptr;
	ID3D11ShaderResourceView* m_shaderResource = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	

	std::vector<unsigned char> m_textureData;
	HRESULT CreateShaderSampler(ID3D11Device* pd3dDevice);
	HRESULT SetShaderSampler(ID3D11Device* pd3dDevice);
	/*From the tutorial
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11RasterizerState* m_rasterState;
	HRESULT CreateRasterState(
		ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pImmediateContext, 
		D3D11_RASTERIZER_DESC& descRastr);*/
	//
	ID3D11SamplerState* m_sampleState = nullptr;
	//
};
HRESULT Texture::CreateShaderSampler(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampler;

	//Here we declare the type of filtering
	sampler.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler.MipLODBias = 0.0f;
	sampler.MaxAnisotropy = 1;
	sampler.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler.BorderColor[0] = 0;
	sampler.BorderColor[1] = 0;
	sampler.BorderColor[2] = 0;
	sampler.BorderColor[3] = 0;
	sampler.MinLOD = 0;
	sampler.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = pd3dDevice->CreateSamplerState(&sampler, &m_sampleState);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;

}

HRESULT Texture::SetShaderSampler(ID3D11Device* pd3dDevice)
{
	//TO DO: Pass information of register and how many times
	HRESULT hr = E_FAIL;

	ID3D11DeviceContext* immContext;
	pd3dDevice->GetImmediateContext(&immContext);
	//First parameter is the register where you are using the texture
	//second is how many times we are going to do this
	immContext->PSSetSamplers(0, 1, &m_sampleState);

	return hr;
}
HRESULT Texture::CreateShaderResourceView(ID3D11Device* pd3dDevice)
{
	HRESULT hr = E_FAIL;
	hr = pd3dDevice->CreateShaderResourceView(m_pd3dTexture2D, nullptr, &m_shaderResource);

	if (FAILED(hr))
	{
		throw std::exception(
			"Texture::CreateShaderResourceView - Falló al crear CreateShaderResourceView");
	}
	return hr;
}
void Texture::SetShaderResourceView(ID3D11Device* pd3dDevice)
{
	ID3D11DeviceContext* immContext;
	pd3dDevice->GetImmediateContext(&immContext);

	immContext->PSSetShaderResources(0, 1, &m_shaderResource);

}
HRESULT Texture::CreateRenderTargetView(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain)
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
	if (FAILED(hr))
	{
		//throw exception
		return hr;
	}

	hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();
	
	return hr;
	
}
HRESULT Texture::CreateDSTDescriptor(
	ID3D11Device* pd3dDevice, 
	ID3D11DeviceContext* pImmediateContext, 
	int width, 
	int height)
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

	hr = pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pd3dTexture2D);

	//D3D11_TEXTURE2D_DESC descDepth;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

	memset(&descDSV, 0, sizeof(descDSV));

	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = pd3dDevice->CreateDepthStencilView(m_pd3dTexture2D, &descDSV, &m_DepthStencilView);
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
HRESULT Texture::LoadFromFile(
	const char* fileName,
	ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pImmediateContext)
{
	D3D11_TEXTURE2D_DESC descTexture;
	int byteperpixel = 0;
	auto m_fileData = stbi_load(fileName, &m_width, &m_height, &byteperpixel, 4);
	HRESULT hr = S_OK;

	if (!m_fileData)
	{
		//throw std::exception(
		//	"Texture::LoadFromFile - No se encontró la textura. Cargando checkers..");
		stbi_image_free(m_fileData);
		m_fileData = stbi_load("checkers.jpg", &m_width, &m_height, &byteperpixel, 4);
	}

	//Have a case if theres no texture cause the idiot didnt make it fine

	memset(&descTexture, 0, sizeof(descTexture));

	descTexture.Width = m_width;
	descTexture.Height = m_height;
	descTexture.MipLevels = 1;
	descTexture.ArraySize = 1;
	descTexture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descTexture.SampleDesc.Count = 1;
	descTexture.SampleDesc.Quality = 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	descTexture.CPUAccessFlags = 0;
	descTexture.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA texturedataBuffer;

	memset(&texturedataBuffer, 0, sizeof(texturedataBuffer));
	//std::vector<unsigned char> dataBuffer;
	//dataBuffer.resize(m_width*m_height*byteperpixel);
	//Nuestro vector tiene el tamaño de todos los pixeles que se encuentran
	//En la textura multiplicado por cuanta informacion tiene cada uno
	///
	//std::memcpy(&dataBuffer[0], &m_fileData[0], m_width*m_height*byteperpixel);
	//Ahora estamos pasando la información de nuestro unsigned char a un vector con memcpy

	//memmove(&texturedataBuffer, &m_fileData[0], m_height*m_width);
	//texturedataBuffer.pSysMem = &m_fileData[0];
	texturedataBuffer.pSysMem = m_fileData; //dataBuffer.data();
	texturedataBuffer.SysMemPitch = m_width *4;
	///
	hr = pd3dDevice->CreateTexture2D(&descTexture, &texturedataBuffer, &m_pd3dTexture2D);
	if (FAILED(hr))
	{
		return hr;	
	}
	stbi_image_free(m_fileData);



	CreateShaderResourceView(pd3dDevice);
	SetShaderResourceView(pd3dDevice);


	return hr;
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