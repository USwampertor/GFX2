#pragma once
class VertexType
{
public:
	struct vertex 
	{
		vertex() : x(0), y(0), z(0), w(0) {};
		vertex(float vx, float vy, float vz, float vw) : x(vx), y(vy), z(vz), w(vw) {}
		float x, y, z, w;
	};
	struct color
	{
		color() : R(0), G(0), B(0), A(0){};
		color(int cr,int cg, int cb, int ca) : R(cr), G(cg), B(cb), A(ca) {}
		int R, G, B, A;
	};
	vertex m_vertex;
	color m_color;
	VertexType()
	{

	}
	~VertexType()
	{
		Clear();
	}
	void Clear()
	{
		m_vertex = { 0.0f,0.0f,0.0f,0.0f };
		m_color = { 0,0,0,0 };
	}
};

