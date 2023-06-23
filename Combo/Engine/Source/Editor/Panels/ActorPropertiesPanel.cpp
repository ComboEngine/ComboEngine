#include "pch.h"
#include "ActorPropertiesPanel.h"
#include "../Editor.h"
#include <Core/Camera.h>
#include <Core/Core.h>
#include <Core/Script.h>
#include <Core/Renderer.h>
#include <imgui_stdlib.h>

std::string ActorPropertiesPanel::GetName()
{
	return "Actor Properties";
}

void ActorPropertiesPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void ActorPropertiesPanel::RenderComponent(std::string name, Component* component) {
	if (name == "Script") {
		Script* script = reinterpret_cast<Script*>(component);
		ImGui::Text("Script Name: Counter");
	}
	if (name == "Renderer") {
		Renderer* renderer = reinterpret_cast<Renderer*>(component);
		ImGui::Text("Mesh");
		ImGui::SameLine();
		std::string text;
		if (renderer->mesh == nullptr) {
			text = "None";
		}
		else {
			text = renderer->mesh->GetName();
		}
		ImGui::InputText("##MeshUUIDBuffer", &text, ImGuiInputTextFlags_ReadOnly);

		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh");
			if (payload != nullptr) {
				renderer->mesh = Core::s_Project.Assets[(const char*)payload->Data];
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Text("Global Material");
		std::string materialText;
		if (renderer->material == nullptr) {
			materialText = "None";
		}
		else {
			materialText = renderer->material->GetName();
		}
		ImGui::SameLine();
		ImGui::InputText("##GlobalMaterial", &materialText, ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Material");
			if (payload != nullptr) {
				renderer->material = Core::s_Project.Assets[(const char*)payload->Data];
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::Separator();

		if (renderer->mesh != nullptr && ImGui::CollapsingHeader("Materials")) {
			int index = 0;
			for (Submesh* submesh : std::any_cast<Mesh*>(renderer->mesh->GetHandle())->Submeshes) {
				std::string submeshMaterialName = "None";
				if (submesh->Material != nullptr) {
					submeshMaterialName = submesh->Material->GetName();
				}
				ImGui::Text((submesh->Name + " [" + std::to_string(index) + "]").c_str());
				ImGui::SameLine();
				ImGui::InputText((std::string("##") + submesh->Name + "Material").c_str(), &submeshMaterialName, ImGuiInputTextFlags_ReadOnly);

				if (ImGui::BeginDragDropTarget()) {
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Material");
					if (payload != nullptr) {
						submesh->Material = Core::s_Project.Assets[(const char*)payload->Data];
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::Separator();
				index++;
			}
		}
	}
	ImGui::Separator();
}


void ActorPropertiesPanel::Draw()
{
	for (int i = 0; i < Core::Scene.Actors.size(); i++) {
		Actor* actor = Core::Scene.Actors[i];
		if (actor->ActorSelected) {
			ImGui::Text("Name: ");
			ImGui::SameLine();
			ImGui::InputText(std::string("##" + std::to_string(i)).c_str(), &actor->Name);

			DrawVec3Control("Position", actor->Position);
			DrawVec3Control("Orientation", actor->Orientation);
			DrawVec3Control("Scale", actor->Scale);

			for (Component* component : actor->Components) {
				std::string name = component->GetName();
				if (ImGui::CollapsingHeader(name.c_str()))
				{
					RenderComponent(name, component);
				}
			}

			ImGui::Separator();
		}
	}
}
