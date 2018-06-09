#pragma once
#include "Graphics_Buffer.h"
#include "VertexType.h"
#include <d3d11.h>
class Mesh
{
public:
	VertexBuffer<VertexType> m_vertexBuffer;
	IndexBuffer<int> m_indexBuffer;
	Mesh()= default;
	~Mesh()
	{

	}
	void Render(ID3D11DeviceContext* pd3dImmediateContext)
	{
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(VertexType);
		offset = sizeof(float);
		pd3dImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer.m_pBuffer , &stride, &offset);
		pd3dImmediateContext->IASetIndexBuffer(m_indexBuffer.m_pBuffer, DXGI_FORMAT_R32_UINT, offset);
		pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//pd3dImmediateContext->DrawIndexed(3, 0, 0);
	}
};

class ExampleTriangle : public Mesh
{
public:
	
	ExampleTriangle()
	{
	}
	~ExampleTriangle()
	{
	}
	void InitializeTriangle(ID3D11Device* pd3dDevice)
	{
		VertexType pVertex;
		pVertex.m_vertex = { -1.0f,-1.0f,0.0f,0.0f };
		pVertex.m_color = { 0.0f,1.0f,0.0f,1.0f };
		m_vertexBuffer.Add(pVertex);
		pVertex.m_vertex = { 0.0f,1.0f,0.0f,0.0f };
		pVertex.m_color = { 1.0f,0.0f,0.0f,1.0f };
		m_vertexBuffer.Add(pVertex);
		pVertex.m_vertex = { 1.0f,-1.0f,0.0f,0.0f };
		pVertex.m_color = { 0.0f,0.0f,1.0f,1.0f };
		m_vertexBuffer.Add(pVertex);
		m_indexBuffer.Add(0);
		m_indexBuffer.Add(1);
		m_indexBuffer.Add(2);
		m_vertexBuffer.CreateHardWareBuffer(pd3dDevice);
		m_indexBuffer.CreateHardWareBuffer(pd3dDevice);
	
	}
	
};
