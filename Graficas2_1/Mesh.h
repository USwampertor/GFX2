#pragma once
#include "Graphics_Buffer.h"
#include "VertexType.h"
#include <d3d11.h>
class Mesh
{
public:
	
	Mesh()= default;
	~Mesh()
	{

	}
};

class ExampleTriangle : public Mesh
{
public:
	VertexBuffer<VertexType*> m_vertexBuffer;
	IndexBuffer<int> m_indexBuffer;
	ExampleTriangle()
	{
	}
	~ExampleTriangle()
	{
	}
	void InitializeTriangle(ID3D11Device* pd3dDevice)
	{
		VertexType* pVertex = new VertexType();
		pVertex->m_vertex = { -1.0f,-1.0f,0.0f,0.0f };
		pVertex->m_vertex = { 0,1,0,1 };
		m_vertexBuffer.Add(pVertex);
		pVertex->m_vertex = { 0.0f,1.0f,0.0f,0.0f };
		pVertex->m_vertex = { 1,0,0,1 };
		m_vertexBuffer.Add(pVertex);
		pVertex->m_vertex = { 1.0f,-1.0f,0.0f,0.0f };
		pVertex->m_vertex = { 0,0,1,1 };
		m_vertexBuffer.Add(pVertex);
		m_indexBuffer.Add(0);
		m_indexBuffer.Add(1);
		m_indexBuffer.Add(2);
		m_vertexBuffer.CreateHardWareBuffer(pd3dDevice);
		m_indexBuffer.CreateHardWareBuffer(pd3dDevice);
		delete pVertex;
	}
	void Render(ID3D11DeviceContext* pd3dImmediateContext)
	{
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(VertexType);
		offset = 0;
		pd3dImmediateContext->IAGetVertexBuffers(0, 1, &m_vertexBuffer.m_pBuffer, &stride, &offset);
		pd3dImmediateContext->IASetIndexBuffer(m_indexBuffer.m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
		pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
};
