#pragma once
#include <d3d11.h>
#include <string>
class Shader
{
public:
	BLOB shaderBlob;
	Shader();
	~Shader();
	void Compile(std::string filename, std::string entryPoint, std::string profile, int FLAGS)
	{

	}
};

class VertexShader : public Shader
{
	ID3D11VertexShader* m_VertexShader;

};

class PixelShader : public Shader
{
	ID3D11PixelShader* m_PixelShader;
};