#pragma once
#include <d3d11.h>
#include <vector>
#include <exception>
#include "InputLayout.h"

class GraphicsBuffer
{
public:
///We gonna make a virtual destructor because we will create derivate classes from this one, and so each class will provide its own destructor

	GraphicsBuffer() = default;
	virtual ~GraphicsBuffer()
	{
		if (m_pBuffer)
		{
			m_pBuffer->Release();
		}
	}
///

	ID3D11Buffer* m_pBuffer = nullptr;

	//VertexBuffer
	//IndexBuffer
};


template<typename TVERTEX>
class VertexBuffer final : public GraphicsBuffer
{
public:
	VertexBuffer() = default;
	~VertexBuffer()
	{
		Clear();
	}

	void Reserve(size_t numObjects)
	{
		m_vertexData.reserve(numObjects);
	}

	void Add(const TVERTEX& vertex)
	{
		m_vertexData.push_back(vertex);
	}
	void Add(std::vector<TVERTEX>& vertices)
	{
		m_vertexData.insert(m_vertexData.end(), vertices.begin(),vertices.end());
	}
	void Add(TVERTEX* pVertices, size_t numVer)
	{
		m_vertexData.insert(m_vertexData.end(),pVertices,pVertices+numVer);
	}
	void Clear()
	{
		m_vertexData.clear();
	}
	void CreateHardWareBuffer(ID3D11Device* pd3dDevice, D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
	{
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(bd));

		bd.Usage = usage;
		bd.ByteWidth = static_cast<UINT32>(sizeof(TVERTEX)*m_vertexData.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		memset(&InitData, 0, sizeof(InitData));
		InitData.pSysMem = &m_vertexData[0];

		HRESULT hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pBuffer);
		if (FAILED(hr))
		{
			throw std::exception("Failed to create Vertex Buffer you idiot");
		}
	}
	void Write(ID3D11DeviceContext* pDeviceContext, void* pData, size_t numBytes)
	{
		D3D11_MAPPED_SUBRESOURCE subRes;
		memset(&subRes, 0, sizeof(subRes));
		//from this buffer we wanna do something
		pDeviceContext->Map(m_pBuffer, 0, nullptr, D3D11_MAP_DISCARD, &subRes);
		//here we write all the shit we need, the thing is that this is not relative to anything specific


		//we later discard the original thing because we dont need it anymore
		pDeviceContext->Unmap(m_pBuffer, 0);
	}
	
private:
	std::vector<TVERTEX> m_vertexData;
};

template <typename TINDEX>
class IndexBuffer final : public GraphicsBuffer
{
public:
	IndexBuffer() = default;
	~IndexBuffer()
	{
		Clear();
	}

	std::size_t Size()
	{
		return m_indexData.size();
	}

	void Reserve(size_t numObjects)
	{
		m_indexData.reserve(numObjects);
	}

	void Add(const TINDEX& index)
	{
		m_indexData.push_back(index);
	}
	void Add(std::vector<TINDEX>& indices)
	{
		m_indexData.insert(m_indexData.end(), indices.begin(), indices.end());
	}
	void Add(TINDEX* pIndices, size_t numInd)
	{
		m_indexData.insert(m_indexData.end(), pIndices, pIndices + numInd);
	}
	void Clear()
	{
		m_indexData.clear();
	}
	void CreateHardWareBuffer(ID3D11Device* pd3dDevice, D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
	{
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(bd));

		bd.Usage = usage;
		bd.ByteWidth = static_cast<UINT32>(sizeof(TINDEX)*m_indexData.size());
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		memset(&InitData, 0, sizeof(InitData));
		InitData.pSysMem = &m_indexData[0];

		HRESULT hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pBuffer);
		if (FAILED(hr))
		{
			throw std::exception("Failed to create Index Buffer you idiot");
		}
	}

	void Write(ID3D11DeviceContext* pDeviceContext , void* pData, size_t numBytes)
	{
		D3D11_MAPPED_SUBRESOURCE subRes;
		memset(&subRes, 0, sizeof(subRes));
		//from this buffer we wanna do something
		pDeviceContext->Map(m_pBuffer, 0, nullptr, D3D11_MAP_DISCARD, &subRes);
		//here we write all the shit we need, the thing is that this is not relative to anything specific


		//we later discard the original thing because we don't need it anymore
		pDeviceContext->Unmap(m_pBuffer, 0);
	}
private:
	std::vector<TINDEX> m_indexData;
};

template <typename TCONSTANT>

class ConstantBuffer final : public GraphicsBuffer
{
public:
	ConstantBuffer() = default;
	~ConstantBuffer()
	{
		Clear();
	}

	void Reserve(size_t numObjects)
	{
		m_constantData.reserve(numObjects);
	}
	
	void Add(const TCONSTANT& constant)
	{
		m_constantData.push_back(constant);
	}
	void Add(std::vector<TCONSTANT>& constants)
	{
		m_constantData.insert(m_constantData.end(), constants.begin(), constants.end());
	}
	void Add(TCONSTANT* pConstants, size_t numConst)
	{
		m_constantData.insert(m_constantData.end(), pConstants, pConstants + numConst);
	}
	void Clear()
	{
		m_constantData.clear();
	}
	void CreateHardWareBuffer(ID3D11Device* pd3dDevice, unsigned int usage = D3D11_USAGE_DEFAULT)
	{
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(bd));

		bd.Usage = usage;
		bd.ByteWidth = static_cast<UINT32>(sizeof(TCONSTANT)*m_constantData.size());
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		memset(&InitData, 0, sizeof(InitData));
		InitData.pSysMem = &m_constantData[0];

		HRESULT hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pBuffer);
		if (FAILED(hr))
		{
			throw std::exception("Failed to create Constant Buffer you idiot");
		}
	}

	void Write(ID3D11DeviceContext* pDeviceContext, void* pData, size_t numBytes)
	{
		D3D11_MAPPED_SUBRESOURCE subRes;
		memset(&subRes, 0, sizeof(subRes));
		//from this buffer we wanna do something
		pDeviceContext->Map(m_pBuffer, 0, nullptr, D3D11_MAP_DISCARD, &subRes);
		//here we write all the shit we need, the thing is that this is not relative to anything specific


		//we later discard the original thing because we dont need it anymore
		pDeviceContext->Unmap(m_pBuffer, 0);
	}
private:
	std::vector<TCONSTANT> m_constantData;
};