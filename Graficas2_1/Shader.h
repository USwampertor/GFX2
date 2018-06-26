#pragma once
#include <d3d11.h>
#include <string>
#include "Device.h"
#include "Graphics_Buffer.h"
#include <d3dcompiler.h>
#include <fstream>
#include <iostream>
class Shader
{	
public:
	ID3DBlob* m_d3dBlob = nullptr;
	//BLOB* m_shaderBlob = nullptr;
	Shader() = default;
	virtual ~Shader()
	{
		if (m_d3dBlob)
		{
			m_d3dBlob->Release();
			m_d3dBlob = nullptr;
		}
	}

	virtual void SetShader(ID3D11DeviceContext* pImmediateDevice) = 0;
	HRESULT Compile(std::string filename, std::string entryPoint, std::string profile, int FLAGS)
	{
		HRESULT hr = S_OK;
		ID3DBlob* error = nullptr;
		std::ifstream file(filename.c_str());
		std::string shadersource;
		if (!file.is_open())
		{
			hr = S_FALSE;
			return hr;
		}
		shadersource = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		file.close();

#ifdef DEBUG
		FLAGS |= D3DCOMPILE_DEBUG;
#endif

		hr = D3DCompile(shadersource.c_str(), shadersource.size(), filename.c_str(), 0, 0,entryPoint.c_str(), profile.c_str(), FLAGS, 0, &m_d3dBlob, &error);
		if (error)
		{
			std::cout << "Hubo un error" << error->GetBufferPointer() << std::endl;
			error->Release();
		}
		return hr;
	}

	const ID3DBlob* getBlob()
	{
		return m_d3dBlob;
	}
};

class VertexShader : public Shader
{
public:
	ID3D11VertexShader* m_VertexShader;
	VertexShader()
	{

	}
	~VertexShader()
	{
		if (m_VertexShader)
		{
			m_VertexShader->Release();
			m_VertexShader = nullptr;
		}
	}

	void CreateFromFile(ID3D11Device* pDevice, const char* fileName, const char* entryPoint)
	{
		HRESULT hr;
		hr = Compile(fileName, entryPoint, "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS);
		if (FAILED(hr))
		{
			throw std::exception("Failed to compile Vertex Shader");
		}

		hr = pDevice->CreateVertexShader(m_d3dBlob->GetBufferPointer(),
									m_d3dBlob->GetBufferSize(),
									nullptr,
									&m_VertexShader);
		if (FAILED(hr))
		{
			throw std::exception("Failed to create Vertex Shader");
		}
	}

	void SetShader(ID3D11DeviceContext* pImmediateDevice)
	{
		pImmediateDevice->VSSetShader(m_VertexShader,0,0);
	}
};

class PixelShader : public Shader
{
public:
	ID3D11PixelShader* m_PixelShader;
	PixelShader()
	{

	}
	~PixelShader()
	{
		if (m_PixelShader)
		{
			m_PixelShader->Release();
			m_PixelShader = nullptr;
		}
	}

	void CreateFromFile(ID3D11Device* pDevice, const char* fileName, const char* entryPoint)
	{
		HRESULT hr;
		hr = Compile(fileName, entryPoint, "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS);
		if (FAILED(hr))
		{
			throw std::exception("Failed to compile Pixel Shader");
		}

		hr = pDevice->CreatePixelShader(m_d3dBlob->GetBufferPointer(),
								   m_d3dBlob->GetBufferSize(),
								   nullptr,
								   &m_PixelShader);

		if (FAILED(hr))
		{
			throw std::exception("Failed to create Pixel Shader");
		}
	}

	void SetShader(ID3D11DeviceContext* pImmediateDevice)
	{
		pImmediateDevice->PSSetShader(m_PixelShader, 0, 0);
	}
};