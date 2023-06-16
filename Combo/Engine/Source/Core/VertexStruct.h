#pragma once


struct Vertex { 
	float X, Y, Z; 
	float texCoordX, texCoordY;
	float normalX, normalY, normalZ, normalW;
	Vertex(float X, float Y, float Z, float texCoordX, float texCoordY, float normalX, float normalY, float normalZ, float normalW) : X(X), Y(Y), Z(Z), texCoordX(texCoordX), texCoordY(texCoordY), normalX(normalX), normalY(normalY), normalZ(normalZ),normalW(normalW) {
	}
	Vertex() {}
};