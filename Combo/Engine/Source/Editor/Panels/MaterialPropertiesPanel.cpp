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

void RenderMaterialProperty(MaterialColor* color,std::string name,Asset** Handle) {
	void* image = nullptr;
	Asset* ptr = *Handle;
	if (ptr != nullptr) {
		image = std::any_cast<Texture*>(ptr->GetHandle())->GetImGuiImage();
		color->UseTexture = true;
		color->ColorTexture = ptr;
		ImGui::ImageButton(image, ImVec2(64, 64));
	}
	else {
		ImGui::Button("Empty", ImVec2(64, 64));
	}

	if (ImGui::BeginDragDropTarget()) {
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture");
		if (payload != nullptr) {
			*Handle = Core::s_Project.Assets[(const char*)payload->Data];
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	ImGui::Text("Diffuse");
	ImGui::SameLine();
	ImGui::ColorEdit4("##DiffuseColor", glm::value_ptr(color->Color));
	ImGui::SameLine();
	if (ImGui::Button("Clear Texture")) {
		*Handle = nullptr;
		color->ColorTexture = nullptr;
		color->UseTexture = false;
	}
	ImGui::Separator();
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
		RenderMaterialProperty(&materialHandle->Diffuse, "Diffuse", &diffuseTexture);
		
	}
	if (ImGui::Button("Save")) {
		this->material->Save();
	}
}
