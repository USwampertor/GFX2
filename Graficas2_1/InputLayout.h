#pragma once
#include <d3d11.h>
#include <vector>
class InputLayout
{
public:
	
public:
	InputLayout();
	~InputLayout();
	void Reserve(size_t numObjects);
	std::vector<D3D11_INPUT_ELEMENT_DESC> descvector;
};

