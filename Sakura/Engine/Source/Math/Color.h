#pragma once

#include <Platform/Windows/IncludeDirectXHeaders.h>

class Color32 {
public:
	float R, G, B, A;
	Color32(float R, float G, float B, float A) : R(R), G(G), B(B), A(A) {
		this->R = R;
		this->G = G;
		this->B = B;
		this->A = A;
	}

	D3DXCOLOR GetD3DXColor() {
		return D3DXCOLOR(R, G, B, A);
	}
};