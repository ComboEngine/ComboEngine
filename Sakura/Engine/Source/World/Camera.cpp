#include "pch.h"
#include "Camera.h"
#include <Platform/Input.h>


void Camera::UpdateCamera()
{
    float moveX = sin(XMConvertToRadians(this->transform->GetOrientation().X)) * 0.05f;
    float moveZ = cos(XMConvertToRadians(this->transform->GetOrientation().Y)) * 0.05f;
    float moveY = sin(XMConvertToRadians(this->transform->GetOrientation().Z)) * 0.05f;

    if (Input::IsKeyDown(DIK_S)) {
        this->transform->SetPosition(this->transform->GetPosition() + Vector3(-moveX, moveY, -moveZ));
    }
    if (Input::IsKeyDown(DIK_W)) {
        this->transform->SetPosition(this->transform->GetPosition() + Vector3(moveX, -moveY, moveZ));
    }
    if (Input::IsKeyDown(DIK_A)) {
        this->transform->SetPosition(this->transform->GetPosition() + Vector3(-moveZ, 0, moveX));
    }
    if (Input::IsKeyDown(DIK_D)) {
        this->transform->SetPosition(this->transform->GetPosition() + Vector3(moveZ, 0, -moveX));
    }

    Vector3 delta = Vector3(-Input::GetMouseDelta().Y*0.05f, -Input::GetMouseDelta().X * 0.05f, 0);
    Vector3 orientation = this->transform->GetOrientation();
    std::cout << "Delta X" << delta.X << std::endl;
    std::cout << "Orientation X"  << orientation.X << std::endl;
    this->transform->SetOrientation(orientation + delta);
}

XMMATRIX Camera::CalculateMatrix()
{
    XMVECTOR position = XMLoadFloat3(&this->transform->GetPosition().ToGraphicsAPIVector());
    XMFLOAT3 rotationVec = XMFLOAT3(
        XMConvertToRadians(this->transform->GetOrientation().GetX()),
        XMConvertToRadians(this->transform->GetOrientation().GetY()),
        XMConvertToRadians(this->transform->GetOrientation().GetZ())
    );
    XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotationVec));
    XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuaternion);
    XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);
    XMMATRIX viewMatrix = XMMatrixMultiply(translationMatrix, rotationMatrix);
    XMVECTOR determinant = XMMatrixDeterminant(viewMatrix);
    viewMatrix = XMMatrixInverse(&determinant, viewMatrix);
    return viewMatrix;
}

