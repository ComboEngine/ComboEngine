#pragma once
#include "pch.h"


class Vector3 {
public:
	float X, Y, Z;
	Vector3() {};
	Vector3(float X, float Y, float Z) {
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	float GetX() {
		return this->X;
	}
	float GetY() {
		return this->Y;
	}
	float GetZ() {
		return this->Z;
	}
	void SetX(float X) {
		this->X = X;
	}
	void SetY(float Y) {
		this->Y = Y;
	}
	void SetZ(float Z) {
		this->Z = Z;
	}
	XMFLOAT3 ToGraphicsAPIVector() {
		return XMFLOAT3(X, Y, Z);
	}
};