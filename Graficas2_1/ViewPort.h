#pragma once
#include "Device.h"
#include <d3d11.h>
#include <DirectXMath.h>
class ViewPort
{
public:
	D3D11_VIEWPORT m_d3dViewPort;
	void CreateViewPort(float width, float height, float topLeftX, float topLeftY);
	void SetViewPort(ID3D11DeviceContext* pImmediateContext);
	ViewPort();
	~ViewPort();
private:
	float
		minDepth = 0,
		maxDepth = 1;
};
void ViewPort::CreateViewPort(float width, float height, float topLeftX, float topLeftY)
{
	m_d3dViewPort.TopLeftX = topLeftX;
	m_d3dViewPort.TopLeftY = topLeftY;
	m_d3dViewPort.Width =  width;
	m_d3dViewPort.Height = height;
}
void ViewPort::SetViewPort(ID3D11DeviceContext* pImmediateContext)
{
	pImmediateContext->RSSetViewports(1, &m_d3dViewPort);
}
ViewPort::ViewPort()
{
}


ViewPort::~ViewPort()
{
}

