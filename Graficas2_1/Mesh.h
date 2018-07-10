#pragma once
#include "Graphics_Buffer.h"
#include "VertexType.h"
#include <d3d11.h>
#include <vector>

using std::vector;

class Mesh
{
public:
	VertexBuffer<VertexType> m_vertexBuffer;
	IndexBuffer<unsigned int> m_indexBuffer;

	Mesh() = default;
	virtual ~Mesh() = default;

	void Render(ID3D11DeviceContext* pd3dImmediateContext)
	{
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(VertexType);
		offset = 0;//sizeof(float);
		pd3dImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer.m_pBuffer , &stride, &offset);
		pd3dImmediateContext->IASetIndexBuffer(m_indexBuffer.m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
		pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pd3dImmediateContext->DrawIndexed(m_indexBuffer.Size(), 0, 0);
	}
};

class Model
{
protected:
	ID3D11Device* m_pd3dDevice = nullptr;
	vector<Mesh> m_meshes;

public:
	Model() = default;
	Model(ID3D11Device* pd3dDevice) : m_pd3dDevice(pd3dDevice) {}
	~Model() = default;

	void SetDevice(ID3D11Device* pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;
	}

	void Render()
	{
		ID3D11DeviceContext* pImmContext;

		m_pd3dDevice->GetImmediateContext(&pImmContext);

		for (auto& mesh : m_meshes)
		{
			mesh.Render(pImmContext);
		}
	}

	void LoadFromFile(const char*) {};

	void CreateTriangle()
	{
		m_meshes.emplace_back();
		auto& mesh = m_meshes.back();

		mesh.m_vertexBuffer.Reserve(3);
		mesh.m_indexBuffer.Reserve(3);

		VertexType pVertex;
		
		pVertex.position = { -1.0f,-1.0f,0.0f,1.0f };
		pVertex.color = { 0.0f,1.0f,0.0f,1.0f };
		mesh.m_vertexBuffer.Add(pVertex);

		pVertex.position = { 0.0f,1.0f,0.0f,1.0f };
		pVertex.color = { 1.0f,1.0f,1.0f,1.0f };
		mesh.m_vertexBuffer.Add(pVertex);

		pVertex.position = { 1.0f,-1.0f,0.0f,1.0f };
		pVertex.color = { 0.0f,0.0f,1.0f,1.0f };
		mesh.m_vertexBuffer.Add(pVertex);

		

		mesh.m_indexBuffer.Add(0);
		mesh.m_indexBuffer.Add(1);
		mesh.m_indexBuffer.Add(2);

		mesh.m_vertexBuffer.CreateHardWareBuffer(m_pd3dDevice);
		mesh.m_indexBuffer.CreateHardWareBuffer(m_pd3dDevice);
	}
	
};
