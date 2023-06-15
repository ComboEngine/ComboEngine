#pragma once


struct Vertex { 
	float X, Y, Z; 
	Color Color;
	Vertex(float X, float Y, float Z, float R, float G, float B, float A) : X(X), Y(Y), Z(Z), Color(R,G,B,A) {
	}
};