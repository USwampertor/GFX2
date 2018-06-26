#pragma once
#include <d3d11.h>
#include "Device.h"
#include "Texture.h"
#include "ViewPort.h"
#include "Mesh.h"
#include <DirectXMath.h>
#include "Shader.h"
#include "InputLayout.h"
#include <d3dcompiler.h>
using std::vector;
class Graphics_API
{
public:
	VertexShader m_vShader;
	PixelShader m_pShader;
	InputLayout m_input;
	Device m_Device;
	Texture m_texture;
	ViewPort m_viewPort;
	//std::vector<Mesh> MeshList;
	Model m_Meshlist;
	Graphics_API();
	~Graphics_API();
	HRESULT InitDevice(HWND g_hWnd);
	
	void Render();
private:
	float screenDepth = 1000.0f, screenNear = 0.1f;
	float FOV = 0;
	
};
Graphics_API::Graphics_API()
{
	// Initialize the world matrix to the identity matrix.
	//m_worldMatrix = DirectX::XMMatrixIdentity();
}
Graphics_API::~Graphics_API()
{
}
HRESULT Graphics_API::InitDevice(HWND g_hWnd)
{
	//D3D11_TEXTURE2D_DESC DSTDesc;
	//D3D11_RASTERIZER_DESC RASDesc;
	float FOV = 0;
	float Ratio = 0;
	HRESULT hr = m_Device.InitializeDevice(g_hWnd);
	if (FAILED(hr))
	{
		return hr;
	}
	//CreateRenderTargetView
	hr = m_texture.CreateRenderTargetView(m_Device.m_pd3dDevice, m_Device.m_pSwapChain);
	if (FAILED(hr))
	{
		return hr;
	}
	//CreateDepthStencilTexture descriptor
	hr = m_texture.CreateDSTDescriptor(m_Device.m_pd3dDevice, m_Device.m_pImmediateContext, m_Device.width, m_Device.height);
	if (FAILED(hr))
	{
		return hr;
	}
	//CreateDepthStencilView descriptor
	//hr = m_texture.CreateDSVDescriptor(m_Device.m_pd3dDevice, m_Device.m_pImmediateContext);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}
	//SetDepthviewDescriptor
	//SetRasterDescriptor
	//hr = m_texture.CreateRasterState(m_Device.m_pd3dDevice, m_Device.m_pImmediateContext, RASDesc);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}
	//SetViewPort
	m_viewPort.CreateViewPort(m_Device.width,m_Device.height,1.0f,1.0f);
	//ProjectionMatrix(FOV, Ratio);
	//OrthogonalMatrix(m_Device.width, m_Device.height);
	
	m_vShader.CreateFromFile(m_Device.m_pd3dDevice, "color.vs", "ColorVertexShader");
	m_pShader.CreateFromFile(m_Device.m_pd3dDevice, "color.ps", "ColorPixelShader");

	m_vShader.SetShader(m_Device.m_pImmediateContext);
	m_pShader.SetShader(m_Device.m_pImmediateContext);

	m_input.SetVertex();
	m_input.SetColor();
	m_input.CreateInputBuffer(m_Device.m_pd3dDevice, &m_vShader);

	//ID3D11ShaderReflection* reflection = nullptr;
	//D3D11_SHADER_DESC vShaderDescriptor;
	//hr = D3DReflect(vShader.m_d3dBlob->GetBufferPointer(), vShader.m_d3dBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)reflection);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}
	//hr = reflection->GetDesc(&vShaderDescriptor);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}
	//input.Parametrize(reflection, vShaderDescriptor.InputParameters);
	m_Meshlist.SetDevice(m_Device.m_pd3dDevice);
	m_Meshlist.CreateTriangle();
	return hr;
}
void Graphics_API::Render()
{
	//
	m_vShader.SetShader(m_Device.m_pImmediateContext);
	m_pShader.SetShader(m_Device.m_pImmediateContext);
	m_input.SetLayout(m_Device.m_pImmediateContext);
	//
	float color[4] = { 1.0f,0.0f,0.0f,1.0f };
	m_Device.m_pImmediateContext->ClearRenderTargetView(m_texture.m_pRenderTargetView, color);

	m_Meshlist.Render();

	m_Device.m_pSwapChain->Present(DXGI_SWAP_EFFECT_DISCARD, DXGI_PRESENT_DO_NOT_WAIT);
}
/*void Graphics_API::ProjectionMatrix(float& fov, float& aspectRatio)
{
	fov = 3.141592654f / 4.0f;
	aspectRatio = (float)(m_Device.width / m_Device.height);
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, screenNear, screenDepth);
}
void Graphics_API::OrthogonalMatrix(float width, float height)
{
	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)width, (float)height, screenNear, screenDepth);
}*/

