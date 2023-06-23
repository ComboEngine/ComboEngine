#include "pch.h"
#include "ScenePanel.h"
#include "../Editor.h"
#include <Core/Camera.h>
#include <Core/Renderer.h>
#include <Core/Core.h>

std::string ScenePanel::GetName()
{
	return "Scene";
}

void ScenePanel::Draw()
{
	if (Core::s_Window->IsWindowFocused()) {
		ImGui::BeginTable("#Scene_Hierarchy", 1, ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_Reorderable
			| ImGuiTableFlags_ScrollY, ImGui::GetContentRegionAvail());
		ImGui::TableHeadersRow();

		for (int i = 0; i < Core::Scene.Actors.size(); i++) {
			Actor* actor = Core::Scene.Actors[i];
			ImGui::TableNextColumn();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4);
			ImGui::Selectable((std::string(actor->Name.c_str()) + "##" + std::to_string(i)).c_str(), &actor->ActorSelected, ImGuiSelectableFlags_SpanAllColumns);
		}

		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			ImGui::SetItemDefaultFocus();

			ImGui::Text("Type");
			ImGui::SameLine();
			const char* elements[] = {
				"Empty",
				"Mesh"
			};
			ImGui::Combo("##CreateActorType", &CurrentActorTypeCreateActorPopup, elements, 2);
			ImGui::Separator();
			if (ImGui::Button("Create")) {
				//TODO finish this
				Actor* actor;
				Actor::Create(&actor);

				if (elements[CurrentActorTypeCreateActorPopup] == "Mesh") {
					Renderer* renderer = new Renderer();
					actor->AddComponent(renderer);
				}
				ImGui::CloseCurrentPopup();
				CurrentActorTypeCreateActorPopup = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
				CurrentActorTypeCreateActorPopup = 0;
			}

			ImGui::EndPopup();
		}


		ImGui::EndTable();
	}
}
