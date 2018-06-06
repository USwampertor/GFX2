#pragma once
#include "Device.h"
#include <d3d11.h>
class ViewPort
{
public:
	D3D11_VIEWPORT m_viewPort;
	int
		topleftx,
		toplefty,
		width,
		height;
	void CreateViewPort(Device& dev);
private:
	float
		minDepth = 0,
		maxDepth = 1;
	ViewPort();
	~ViewPort();
};
void ViewPort::CreateViewPort(Device& dev)
{
	//m_viewPort.TopLeftX = 0;
	//m_viewPort.TopLeftY = 0;
	//m_viewPort.Width = dev.width;
	//m_viewPort.Height = dev.height;
	//
	//dev.m_pImmediateContext->RSSetViewports(1, &m_viewPort);
}
ViewPort::ViewPort()
{
}


ViewPort::~ViewPort()
{
}

