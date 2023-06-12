#include "pch.h"
#include "Camera.h"
#include <Platform/Input.h>


void Camera::UpdateCamera()
{
    float moveX = sin(glm::radians(transform->Orientation.x)) * 0.05f;
    float moveZ = cos(glm::radians(transform->Orientation.y)) * 0.05f;
    float moveY = sin(glm::radians(transform->Orientation.z)) * 0.05f;

    if (Input::IsKeyDown(DIK_W)) {
        transform->Move(glm::vec3(-moveX, moveY, -moveZ));
    }
    if (Input::IsKeyDown(DIK_S)) {
        transform->Move(glm::vec3(moveX, -moveY, moveZ));
    }
    if (Input::IsKeyDown(DIK_A)) {
        transform->Move(glm::vec3(-moveZ, 0, moveX));
    }
    if (Input::IsKeyDown(DIK_D)) {
        transform->Move(glm::vec3(moveZ, 0, -moveX));
    }

    glm::vec2 input = Input::GetMouseDelta();

    //transform->Rotate(glm::quat(glm::vec3(glm::radians(input.y), glm::radians(input.x), 0)));

}

glm::mat4 Camera::CalculateMatrix()
{
    auto rotation = this->transform->Orientation;
    auto r = glm::mat4_cast(rotation);
    auto t = glm::translate(glm::mat4(1.f), transform->Position);
    auto view = glm::affineInverse(t * r);
    return view;
}

