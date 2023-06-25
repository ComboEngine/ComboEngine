#include "pch.h"
#include "ScenePanel.h"
#include "../Editor.h"
#include <Core/Camera.h>
#include <Core/Renderer.h>
#include <Core/Lights/DirectionalLight.h>
#include <Core/Lights/PointLight.h>
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
			if (ImGui::BeginPopupContextItem(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
				if (ImGui::MenuItem("Remove")) {
					Core::Scene.RemoveActor(actor);
				}
				if (ImGui::MenuItem("Duplicate")) {
					Actor* actorDuplicated = new Actor(*actor);
					actorDuplicated->UUID = uuid::generate_uuid_v4();
					actorDuplicated->Name = actor->Name + " Copy";
					Core::Scene.Actors.push_back(actorDuplicated);
				}
				ImGui::EndPopup();
			}
		}

		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			ImGui::SetItemDefaultFocus();

			ImGui::Text("Type");
			ImGui::SameLine();
			const char* elements[] = {
				"Empty",
				"Mesh",
				"Directional Light",
				"Point Light"
			};
			ImGui::Combo("##CreateActorType", &CurrentActorTypeCreateActorPopup, elements, 4);
			ImGui::Separator();
			if (ImGui::Button("Create")) {
				//TODO finish this
				Actor* actor;
				Actor::Create(&actor);

				if (elements[CurrentActorTypeCreateActorPopup] == "Mesh") {
					Renderer* renderer = new Renderer();
					actor->AddComponent(renderer);
				}
				if (elements[CurrentActorTypeCreateActorPopup] == "Directional Light") {
					DirectionalLight* light = new DirectionalLight();
					actor->AddComponent(light);
				}
				if (elements[CurrentActorTypeCreateActorPopup] == "Point Light") {
					PointLight* light = new PointLight();
					actor->AddComponent(light);
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
