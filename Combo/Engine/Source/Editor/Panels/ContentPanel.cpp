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
	float offset = 18;
	for (const auto& asset : Core::s_Project.Assets) {
		if (asset.second != nullptr) {			
			float offseting = 0;
			ImGui::SameLine();
			if (asset.second->GetType() == "Texture") {
				ImGui::ImageButton(std::any_cast<Texture*>(asset.second->GetHandle())->GetImGuiImage(), ImVec2(128, 100));
				offseting += 142;
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(ImGui::GetWindowPos().x + offset, ImGui::GetWindowPos().y + 130), ImVec2(ImGui::GetWindowPos().x + offset + 130, 200 + ImGui::GetWindowPos().y), IM_COL32(25, 25, 25, 255));
			}
			else {
				ImGui::Button((std::string("##") + asset.first).c_str(), ImVec2(128, 100));
				offseting += 136;
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(ImGui::GetWindowPos().x + offset, ImGui::GetWindowPos().y + 130), ImVec2(ImGui::GetWindowPos().x + offset + 126, 200 + ImGui::GetWindowPos().y), IM_COL32(25,25,25,255));
			}
			ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetWindowPos().x + offset+10, ImGui::GetWindowPos().y + 140), IM_COL32(200,200,200,255), asset.second->Name.c_str());

			ImGui::SetWindowFontScale(0.945f);
			ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetWindowPos().x + offset+8, ImGui::GetWindowPos().y + 170), IM_COL32(140,140,140, 255), asset.second->GetType().c_str());
			ImGui::SetWindowFontScale(1);

			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload(asset.second->GetType().c_str(), asset.first.c_str(), sizeof(asset.first));
				ImGui::Text(asset.first.c_str());
				ImGui::EndDragDropSource();
			}
			offset += offseting;
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
				Asset::New(&asset, AssetType::MaterialAssetType, PathBuffer);
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
