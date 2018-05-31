#pragma once
class ViewPort
{
public:
	int
		topleftx,
		toplefty,
		width,
		height;
private:
	float
		minDepth = 0,
		maxDepth = 1;
	ViewPort();
	~ViewPort();
};


