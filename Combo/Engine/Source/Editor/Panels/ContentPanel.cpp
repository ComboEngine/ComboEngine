#include "pch.h"
#include "ContentPanel.h"
#include "../Editor.h"
#include <Core/Camera.h>
#include <Core/Core.h>
#include <imgui_stdlib.h>

std::string ContentPanel::GetName()
{
	return "Content";
}

void ContentPanel::Draw()
{
	int index = 0;
	for (const auto& asset : Core::s_Project.Assets) {
		if (asset.second != nullptr) {
			ImGui::SameLine();
			ImGui::Button((asset.second->GetName() + "##" + std::to_string(index)).c_str(), ImVec2(128, 128));

			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload(asset.second->GetType().c_str(), asset.first.c_str(), sizeof(asset.first));
				ImGui::Text(asset.first.c_str());
				ImGui::EndDragDropSource();
			}
		}
		index++;
	}

	if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
		ImGui::SetItemDefaultFocus();

		ImGui::Text("Type");
		ImGui::SameLine();
		const char* elements[] = {
			"Material"
		};
		ImGui::Combo("##CreateActorType", &CurrentAssetTypeCreateAssetPopup, elements, 1);
		ImGui::Text("Path");
		ImGui::SameLine();
		ImGui::InputText("##Path",&PathBuffer);
		ImGui::Separator();
		if (ImGui::Button("Create")) {
			if (elements[CurrentAssetTypeCreateAssetPopup] == "Material") {
				Asset* asset;
				Asset::Import(&asset, PathBuffer, "", NULL);
			}
			ImGui::CloseCurrentPopup();
			PathBuffer = "";
			CurrentAssetTypeCreateAssetPopup = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
			PathBuffer = "";
			CurrentAssetTypeCreateAssetPopup = 0;
		}

		ImGui::EndPopup();
	}
}
