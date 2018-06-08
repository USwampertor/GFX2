#pragma once
#include <d3d11.h>
#include <vector>
class InputLayout
{
public:
	ID3D11InputLayout * m_layout;
	std::vector<D3D11_INPUT_ELEMENT_DESC> descvector;
	InputLayout()
	{

	}
	~InputLayout()
	{

	}
	void Reserve(size_t numObjects)
	{
		descvector.reserve(numObjects);
	}
	void Add(const D3D11_INPUT_ELEMENT_DESC& input)
	{
		descvector.push_back(input);
	}
	void Add(std::vector<D3D11_INPUT_ELEMENT_DESC>& inputs)
	{
		descvector.insert(descvector.end(), inputs.begin(), inputs.end());
	}
	void Add(D3D11_INPUT_ELEMENT_DESC* pInput, size_t numInputs)
	{
		descvector.insert(descvector.end(), pInput, pInput + numInputs);
	}
	void Clear()
	{
		descvector.clear();
	}
	void CreateHardWareBuffer(ID3D11Device* pd3dDevice, BLOB vertexBlob)
	{

		//D3D11_SUBRESOURCE_DATA InitData;
		//memset(&InitData, 0, sizeof(InitData));
		//InitData.pSysMem = &descvector[0];

		HRESULT hr = pd3dDevice->CreateInputLayout(&descvector[0], descvector.size(), vertexBlob.pBlobData, (SIZE_T)vertexBlob.cbSize, &m_layout);
		if (FAILED(hr))
		{
			throw std::exception("Failed to create Vertex Buffer you idiot");
		}
	}
	//void Write(ID3D11DeviceContext* pDeviceContext, void* pData, size_t numBytes)
	//{
	//	D3D11_MAPPED_SUBRESOURCE subRes;
	//	memset(&subRes, 0, sizeof(subRes));
	//	//from this buffer we wanna do something
	//	pDeviceContext->Map(m_pBuffer, 0, nullptr, D3D11_MAP_DISCARD, &subRes);
	//	//here we write all the shit we need, the thing is that this is not relative to anything specific
	//
	//
	//	//we later discard the original thing because we dont need it anymore
	//	pDeviceContext->Unmap(m_pBuffer, 0);
	//}
};

