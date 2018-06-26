#pragma once
struct Vertex
{
	Vertex() = default;
	Vertex(float vx, float vy=0.0f, float vz = 0.0f, float vw = 1.0f)
		: x(vx),
		  y(vy),
		  z(vz),
		  w(vw)
	{}
	
	float x;
	float y;
	float z;
	float w;
};

struct Color
{
	Color() = default;
	Color(float cr, float cg = 0.0f, float cb = 0.0f, float ca = 1.0f)
		: R(cr),
		  G(cg),
		  B(cb),
		  A(ca)
	{}

	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	float A = 1.0f;
};

struct VertexType
{
	Vertex position;
	Color color;
};

