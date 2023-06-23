#include "pch.h"
#include "MaterialPropertiesPanel.h"
#include "../Editor.h"
#include <Core/Camera.h>
#include <Core/Core.h>
#include <imgui_stdlib.h>
#include <Core/Material.h>

std::string MaterialPropertiesPanel::GetName()
{
	return "Material Properties";
}

void MaterialPropertiesPanel::Draw()
{
	ImGui::Text("Edited material");
	ImGui::SameLine();
	std::string text = "None";
	if (material != nullptr) {
		text = material->Name;
	}
	ImGui::InputText("##GlobalMaterial", &text, ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginDragDropTarget()) {
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Material");
		if (payload != nullptr) {
			material = Core::s_Project.Assets[(const char*)payload->Data];
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::Separator();
	if (material != nullptr) {
		Material* materialHandle = std::any_cast<Material*>(material->GetHandle());
		ImGui::Text("Diffuse");
		ImGui::SameLine();
		ImGui::ColorEdit4("##DiffuseColor", glm::value_ptr(materialHandle->Diffuse.Color));
		ImGui::Separator();
	}
}
