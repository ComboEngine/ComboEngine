#pragma once

#include <xnamath.h>

struct Vertex    //Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float u, float v, float nx, float ny,float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx,ny,nz,1) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT4 normal;
};