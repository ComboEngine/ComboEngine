#pragma once
#include "pch.h"
#include "Vector3.h"

class Transform {
private:
	Vector3 Position;
	Vector3 Orientation;
	Vector3 Scale;
public:
	Vector3 GetPosition();
	Vector3 GetOrientation();
	Vector3 GetScale();
	XMMATRIX CalculateMatrix();
	void SetTransform(Vector3 Position, Vector3 Orientation, Vector3 Scale);
	void SetPosition(Vector3 Position);
	void SetOrientation(Vector3 Orientation);
	void SetScale(Vector3 Scale);
};