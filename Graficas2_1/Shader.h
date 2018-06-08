#pragma once
#include <d3d11.h>
#include <string>
#include <d3dcompiler.h>
#include "Device.h"
#include "Graphics_Buffer.h"
#include <fstream>
class Shader
{
private:
	
public:
	ID3DBlob* m_d3dBlob;
	BLOB m_shaderBlob;
	Shader() = default;
	~Shader()
	{

	}
	virtual HRESULT Compile(std::string filename, std::string entryPoint, std::string profile, int FLAGS) = 0;
};

class VertexShader : public Shader
{
	ID3D11VertexShader* m_VertexShader;
	HRESULT Compile(std::string filename, std::string entryPoint, std::string profile, int FLAGS)
	{
		ID3DBlob* error = nullptr;
		std::fstream file;
		std::string shadersource;
		char* str;
		file.open(filename.c_str());
		file.seekg(0, std::ios::end);
		//_itoa(file.tellg(),str,10);
		file.seekg(0, std::ios::beg);
		while (!file.eof())
		{
			std::getline(file, shadersource);
		}
		file.close();
		HRESULT 
			hr = D3DCompile(
				shadersource.c_str(), shadersource.size(), filename.c_str(), 0, 0,
				entryPoint.c_str(), profile.c_str(), FLAGS, 0, &m_d3dBlob ,&error);
		return hr;
	}
	~VertexShader()
	{
		if (m_VertexShader)
		{
			m_VertexShader->Release();
			m_VertexShader = nullptr;
		}
	}
};

class PixelShader : public Shader
{
	ID3D11PixelShader* m_PixelShader;
	HRESULT Compile(std::string filename, std::string entryPoint, std::string profile, int FLAGS)
	{
		ID3DBlob* error = nullptr;
		std::fstream file;
		std::string shadersource;
		char* str;
		file.open(filename.c_str());
		file.seekg(0, std::ios::end);
		//_itoa(file.tellg(), str, 10);
		file.seekg(0, std::ios::beg);
		while (!file.eof())
		{
			std::getline(file, shadersource);
		}
		file.close();
		HRESULT
			hr = D3DCompile(
				shadersource.c_str(), shadersource.size(), filename.c_str(), 0, 0,
				entryPoint.c_str(), profile.c_str(), FLAGS, 0, &m_d3dBlob, &error);
		return hr;
	}
	~PixelShader()
	{
		if (m_PixelShader)
		{
			m_PixelShader->Release();
			m_PixelShader = nullptr;
		}
	}
};