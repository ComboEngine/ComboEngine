#pragma once
#include "pch.h"


class Vector2 {
public:
	float X, Y;
	Vector2() {};
	Vector2(float X, float Y) {
		this->X = X;
		this->Y = Y;
	}

	float GetX() {
		return this->X;
	}
	float GetY() {
		return this->Y;
	}
	void SetX(float X) {
		this->X = X;
	}
	void SetY(float Y) {
		this->Y = Y;
	}
	XMFLOAT2 ToGraphicsAPIVector() {
		return XMFLOAT2(X, Y);
	}
	Vector2 operator+(Vector2 right)
	{
		return Vector2(this->X + right.X, this->Y + right.Y);
	}
	Vector2 operator-(Vector2 right)
	{
		return Vector2(this->X - right.X, this->Y - right.Y);
	}
};