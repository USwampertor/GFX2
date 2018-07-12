#pragma once
class Vector4
{

public:
	float x, y, z, w;
	Vector4() = default;
	Vector4(float vx, float vy = 0.0f, float vz = 0.0f, float vw = 1.0f)
		: x(vx),
		  y(vy),
		  z(vz),
		  w(vw)
	{}
};
//struct Vertex
//{
//	Vertex() = default;
//	Vertex(float vx, float vy=0.0f, float vz = 0.0f, float vw = 1.0f)
//		: x(vx),
//		  y(vy),
//		  z(vz),
//		  w(vw)
//	{}
//	
//	float x;
//	float y;
//	float z;
//	float w;
//};
//
//struct Color
//{
//	Color() = default;
//	Color(float cr, float cg = 0.0f, float cb = 0.0f, float ca = 1.0f)
//		: R(cr),
//		  G(cg),
//		  B(cb),
//		  A(ca)
//	{}
//
//	float R = 0.0f;
//	float G = 0.0f;
//	float B = 0.0f;
//	float A = 1.0f;
//};
//struct Normal
//{
//	Normal() = default;
//	Normal(float x, float y = 0.0f, float z = 0.0f, float w = 1.0f)
//		: nx(x),
//		  ny(y),
//		  nz(z),
//		  nw(w)
//	{}
//
//	float nx;
//	float ny;
//	float nz;
//	float nw;
//};
struct VertexType
{
	Vector4  position;
	Vector4	 color;
	Vector4  normal;
	float u = 0, v = 0;
};

