#pragma once

#include "pch.h"

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

	static Color32 From255Values(float R, float G, float B, float A) {
		return Color32(R / 255.0F, G / 255.0F, B / 255.0F, A / 255.0F);
	}
};