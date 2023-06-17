#include "pch.h"
#include "Camera.h"
#include "Core.h"
#include "Input.h"

glm::vec3 Camera::Position = glm::vec3(0, 0, 3);
glm::vec3 Camera::Orientation = glm::vec3(0, 0, 0);
glm::mat4 Camera::CalculateViewMatrix()
{
	auto rotation = glm::quat(glm::vec3(glm::radians(Orientation.x), glm::radians(Orientation.y), glm::radians(Orientation.z)));
	auto r = glm::mat4_cast(rotation);
	auto t = glm::translate(glm::mat4(1.f), Position);
	auto view = glm::affineInverse(t * r);
	return view;
}

void Camera::Drone()
{
	//Core::s_Window.Get()->LockCursor(true);
	float moveX = sin(glm::radians(Camera::Orientation.x)) * 0.05f;
	float moveZ = cos(glm::radians(Camera::Orientation.y)) * 0.05f;
	float moveY = sin(glm::radians(Camera::Orientation.z)) * 0.05f;

	if (Input::IsKeyDown(COMBO_KEY_W)) {
		Camera::Position += glm::vec3(-moveX, moveY, -moveZ);
	}
	if (Input::IsKeyDown(COMBO_KEY_S)) {
		Camera::Position += glm::vec3(moveX, -moveY, moveZ);
	}
	if (Input::IsKeyDown(COMBO_KEY_A)) {
		Camera::Position += glm::vec3(-moveZ, 0, moveX);
	}
	if (Input::IsKeyDown(COMBO_KEY_D)) {
		Camera::Position += glm::vec3(moveZ, 0, -moveX);
	}

	glm::vec2 delta = Input::GetMouseDelta();
	Camera::Orientation += glm::vec3(-(delta.y * 0.05f), (delta.x * 0.05f), 0);
}

glm::mat4 Camera::CalculateProjectionMatrix()
{
	return glm::perspective(0.4f * 3.14f, (float)Core::s_Window.Get()->GetWidth() / Core::s_Window.Get()->GetHeight(), 0.1f, 1000.0f);
}
