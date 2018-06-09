#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Shader.h"
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
	void SetVertex()
	{
		D3D11_INPUT_ELEMENT_DESC vertex;
		vertex.SemanticName = "POSITION";
		vertex.SemanticIndex = 0;
		vertex.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		vertex.InputSlot = 0;
		vertex.AlignedByteOffset = 0;
		vertex.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		vertex.InstanceDataStepRate = 0;
		Add(vertex);
	}
	void SetColor()
	{
		D3D11_INPUT_ELEMENT_DESC index;
		index.SemanticName = "COLOR";
		index.SemanticIndex = 0;
		index.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		index.InputSlot = 0;
		index.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		index.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		index.InstanceDataStepRate = 0;
		Add(index);
	}
	void Reserve(size_t numObjects)
	{
		descvector.reserve(numObjects);
	}
	void Add(const D3D11_INPUT_ELEMENT_DESC& input)
	{
		descvector.push_back(input);
	}
	void Parametrize(ID3D11ShaderReflection* pReflection, UINT size)
	{
		int offset = 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC* paramDesc = nullptr;
			pReflection->GetInputParameterDesc(i, paramDesc);
			D3D11_INPUT_ELEMENT_DESC inputDesc;
			
			inputDesc.SemanticIndex = paramDesc->SemanticIndex;
			inputDesc.SemanticName = paramDesc->SemanticName;
			inputDesc.InputSlot = D3D11_INPUT_PER_VERTEX_DATA;
			switch (paramDesc->Mask)
			{
			case 1:
				offset += 4;
				if (paramDesc->ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32_UINT;
				if (paramDesc->ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32_SINT;
				if (paramDesc->ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32_FLOAT;
				break;
			case 2:
				offset += 8;
				if (paramDesc->ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32_UINT;
				if (paramDesc->ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32_SINT;
				if (paramDesc->ComponentType ==D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case 3:
				offset += 12;
				if (paramDesc->ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				if (paramDesc->ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				if (paramDesc->ComponentType ==D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case 4:
				offset += 16;
				if (paramDesc->ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				if (paramDesc->ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				if (paramDesc->ComponentType ==D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			}
			inputDesc.AlignedByteOffset = offset;
			descvector.push_back(inputDesc);
		}
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
	void CreateInputBuffer(ID3D11Device* pd3dDevice, Shader* shader)
	{
		
		HRESULT hr = pd3dDevice->CreateInputLayout(&descvector[0], descvector.size(),shader->m_d3dBlob->GetBufferPointer(), shader->m_d3dBlob->GetBufferSize(), &m_layout);
		if (FAILED(hr))
		{
			throw std::exception("Failed to create Vertex Buffer you idiot");
		}
		shader->m_d3dBlob->Release();
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

