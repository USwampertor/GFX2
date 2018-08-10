#pragma once
#include <d3d11.h>
//Next step, Device and device context in own classes, also set has to be in setFunction in API
class Sampler
{
public:
	Sampler();
	~Sampler();
	HRESULT CreateShaderSampler(ID3D11Device* pd3dDevice);
	void SetShaderSampler(ID3D11Device* pd3dDevice);
};

