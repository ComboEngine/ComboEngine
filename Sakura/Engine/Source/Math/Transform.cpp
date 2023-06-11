#include "pch.h"
#include "Transform.h"


glm::mat4 Transform::CalculateMatrix()
{
    //glm::translate() * XMMatrixRotationRollPitchYaw(this->Orientation.X * (3.14159265359f/180.0f), this->Orientation.Y * (3.14159265359f / 180.0f), this->Orientation.Z * (3.14159265359f / 180.0f)) * XMMatrixScaling(this->Scale.X, this->Scale.Y, this->Scale.Z);
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, this->Position);
    matrix = matrix * glm::mat4_cast(this->Orientation);
    matrix = glm::scale(matrix, this->Scale);
    return matrix;
}

void Transform::Rotate(glm::quat orientation)
{
    this->Orientation = this->Orientation + orientation;
}

void Transform::Move(glm::vec3 position)
{
    this->Position = this->Position + position;
}
