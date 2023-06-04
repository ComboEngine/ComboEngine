#include "pch.h"
#include "Transform.h"

Vector3 Transform::GetPosition()
{
    return this->Position;
}

Vector3 Transform::GetOrientation()
{
    return this->Orientation;
}

Vector3 Transform::GetScale()
{
    return this->Scale;
}

XMMATRIX Transform::CalculateMatrix()
{
    return XMMatrixTranslation(this->Position.X,this->Position.Y,this->Position.Z) * XMMatrixRotationRollPitchYaw(this->Orientation.X * (3.14159265359f/180.0f), this->Orientation.Y * (3.14159265359f / 180.0f), this->Orientation.Z * (3.14159265359f / 180.0f)) * XMMatrixScaling(this->Scale.X, this->Scale.Y, this->Scale.Z);
}

void Transform::SetTransform(Vector3 Position, Vector3 Orientation, Vector3 Scale)
{
    this->SetPosition(Position);
    this->SetOrientation(Orientation);
    this->SetScale(Scale);
}

void Transform::SetPosition(Vector3 Position)
{
    this->Position = Position;
}

void Transform::SetOrientation(Vector3 Orientation)
{
    this->Orientation = Orientation;
}

void Transform::SetScale(Vector3 Scale)
{
    this->Scale = Scale;
}
