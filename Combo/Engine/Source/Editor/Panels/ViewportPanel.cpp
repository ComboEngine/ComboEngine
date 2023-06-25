#include "pch.h"
#include "ViewportPanel.h"
#include "../Editor.h"
#include <Core/Camera.h>
#include <Core/Renderer.h>
#include <Core/Core.h>

std::string ViewportPanel::GetName()
{
	return "Viewport";
}

glm::vec3 Scale(const glm::vec3& v, float desiredLength)
{
	return v * desiredLength / length(v);
}


bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale)
{
	//nie rob kometarzy gosciu
	using namespace glm;
	using T = float;

	mat4 LocalMatrix(transform);

	if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
		return false;

	translation = vec3(LocalMatrix[3]);
	LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

	vec3 Row[3];

	for (length_t i = 0; i < 3; ++i)
		for (length_t j = 0; j < 3; ++j)
			Row[i][j] = LocalMatrix[i][j];

	
	scale.x = length(Row[0]);
	Row[0] = Scale(Row[0], static_cast<T>(1));

	
	scale.y = length(Row[1]);
	Row[1] = Scale(Row[1], static_cast<T>(1));

	scale.z = length(Row[2]);
	Row[2] = Scale(Row[2], static_cast<T>(1));

#if _DEBUG
	vec3 Pdum3 = cross(Row[1], Row[2]);
	dot(Row[0], Pdum3) >= static_cast<T>(0);
#endif
	
	int i, j, k = 0;
	T root, trace = Row[0].x + Row[1].y + Row[2].z;
	if (trace > static_cast<T>(0))
	{
		root = sqrt(trace + static_cast<T>(1));
		rotation.w = static_cast<T>(0.5) * root;
		root = static_cast<T>(0.5) / root;
		rotation.x = root * (Row[1].z - Row[2].y);
		rotation.y = root * (Row[2].x - Row[0].z);
		rotation.z = root * (Row[0].y - Row[1].x);
	}
	else
	{
		static int Next[3] = { 1, 2, 0 };
		i = 0;
		if (Row[1].y > Row[0].x) i = 1;
		if (Row[2].z > Row[i][i]) i = 2;
		j = Next[i];
		k = Next[j];

		root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<T>(1.0));

		rotation[i] = static_cast<T>(0.5) * root;
		root = static_cast<T>(0.5) / root;
		rotation[j] = root * (Row[i][j] + Row[j][i]);
		rotation[k] = root * (Row[i][k] + Row[k][i]);
		rotation.w = root * (Row[j][k] - Row[k][j]);
	}

	return true;
}

void ViewportPanel::Draw()
{
	Camera::ProjectionWidth = ImGui::GetContentRegionAvail().x;
	Camera::ProjectionHeight = ImGui::GetContentRegionAvail().y;
	void* image;
	switch (ViewMode) {
	case Position:
		image = Core::s_GBuffer->Position->GetImage(); break;
	case Normal:
		image = Core::s_GBuffer->Normal->GetImage(); break;
	case Diffuse:
		image = Core::s_GBuffer->Diffuse->GetImage(); break;
	case FinalBuffer:
		image = Core::PostFX.Frame->GetImage(); break;
	}
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImGuizmo::DrawGrid(glm::value_ptr(Camera::CalculateViewMatrix()), glm::value_ptr(Camera::CalculateProjectionMatrix()), glm::value_ptr(glm::mat4(1.0f)), 16);


	ImGui::Image((void*)image, ImGui::GetContentRegionAvail());
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
		MouseHooked = true;
	}

	Actor* actor = nullptr;
	for (Actor* actorI : Core::Scene.Actors) {
		if (actorI->ActorSelected && actor == nullptr) {
			actor = actorI;
		}
	}
	if (actor != nullptr) {
		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::translate(Model, actor->Position);
		Model = Model * glm::mat4_cast(glm::quat(glm::vec3(actor->Orientation.x,actor->Orientation.y,actor->Orientation.z)));
		Model = glm::scale(Model, actor->Scale);

		int Operation;
		switch (this->GizmoType) {
		case POSITION: Operation = ImGuizmo::OPERATION::TRANSLATE; break;
		case ROTATION: Operation = ImGuizmo::OPERATION::ROTATE; break;
		case SCALE: Operation = ImGuizmo::OPERATION::SCALE; break;
		case ALL: Operation = ImGuizmo::OPERATION::UNIVERSAL; break;
		}
		if (ImGuizmo::Manipulate(glm::value_ptr(Camera::CalculateViewMatrix()), glm::value_ptr(Camera::CalculateProjectionMatrix()), (ImGuizmo::OPERATION)Operation, ImGuizmo::MODE::WORLD, glm::value_ptr(Model))) {
			glm::vec3 position;
			glm::quat orientation;
			glm::vec3 scale;
			DecomposeTransform(Model, position, orientation, scale);

			actor->Position = position;
			//actor->Orientation = h;
			actor->Scale = scale;
		}
	}
}
