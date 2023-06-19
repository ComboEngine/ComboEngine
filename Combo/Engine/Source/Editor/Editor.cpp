#include "pch.h"
#include "Editor.h"
#include <Core/Core.h>
#include <Core/Mesh.h>
#include <Core/Assets/TextureSerializer.h>
#include <Core/Assets/MeshSerializer.h>
#include <Core/Renderer.h>
#include <Core/Input.h>
#include <Core/Render2D.h>
#include <Core/Camera.h>
#include <Core/Script.h>
#include <imgui_stdlib.h>
#include "Color.h"

bool Editor::MouseHooked;

void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
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

void Editor::Init()
{
	ImGui::GetIO().Fonts->AddFontFromFileTTF("./Content/Roboto.ttf", 15.0f);
	Colours::Set();

	Core::DrawEvent.Hook([&] {
		if (Input::IsKeyDown(COMBO_KEY_ESCAPE)) {
			MouseHooked = false;
		}
		Core::s_Window.Get()->LockCursor(MouseHooked);
		if (MouseHooked) {
			Camera::Drone();
		}
	});

	Core::ImGuiDrawEvent.Hook([&] {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("Dockspace", nullptr, window_flags);
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));

		ImGui::Begin("Viewport");
		Camera::ProjectionWidth = ImGui::GetContentRegionAvail().x;
		Camera::ProjectionHeight = ImGui::GetContentRegionAvail().y;
		if (ImGui::ImageButton((void*)Core::s_Color.Get()->GetImage(), ImGui::GetContentRegionAvail())) {
			MouseHooked = true;
		}
		ImGui::End();

		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar);

		ImGui::BeginTable("#Scene_Hierarchy", 1, ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_Reorderable
			| ImGuiTableFlags_ScrollY, ImGui::GetContentRegionAvail());
		ImGui::TableHeadersRow();

		for (int i = 0;i<Core::Actors.size();i++) {
			Scope<Actor> actor = Core::Actors[i];
			ImGui::TableNextColumn();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4);
			ImGui::Selectable((std::string(actor.Get()->Name.c_str()) + "##" + std::to_string(i)).c_str(), &actor.Get()->ActorSelected, ImGuiSelectableFlags_SpanAllColumns);
		}

		ImGui::EndTable();

		ImGui::End();

		ImGui::Begin("Actor Properties");

		for (int i = 0; i < Core::Actors.size(); i++) {
			Scope<Actor> actor = Core::Actors[i];
			if (actor.Get()->ActorSelected) {
				ImGui::Text("Name: ");
				ImGui::SameLine();
				ImGui::InputText(std::string("##" + std::to_string(i)).c_str(), &actor.Get()->Name);

				DrawVec3Control("Position", actor.Get()->Position);
				DrawVec3Control("Orientation", actor.Get()->Orientation);
				DrawVec3Control("Scale", actor.Get()->Scale);

				for (Component* component : actor.Get()->Components) {
					std::string name = component->GetName();
					if (ImGui::CollapsingHeader(name.c_str()))
					{
						RenderComponent(name,component);
					}
				}

				ImGui::Separator();
			}
		}
		ImGui::End();

		ImGui::Begin("Content");
		ImGui::End();

		ImGui::Begin("Graphics Settings");
		ImGui::End();

		ImGui::End();
	});

	Scope<Actor> actor;
	Actor::Create(actor);

	Scope<Actor> actor2;
	Actor::Create(actor2);

	actor.Get()->Scale = glm::vec3(0.001f, 0.001f, 0.001f);

	Scope<Renderer> renderer;
	Scope<Renderer>::Create(renderer);

	Scope<Script> script;
	Scope<Script>::Create(script);

	actor.Get()->AddComponent(script.Cast<Script>());
	actor.Get()->AddComponent(renderer.Cast<Component>());

	Scope<Mesh> mesh;
	MeshSerializer::Read(mesh, "Content/Sponza.cbmesh");

	renderer.Get()->mesh = mesh;

	Scope<Texture> test;
	TextureSerializer::Read(test, "Content/test.cbtexture");

	Scope<Material> material;
	Material::Create(material);
	material.Get()->Diffuse = MaterialColor::FromColor(glm::vec4(1, 1, 1, 1));

	renderer.Get()->material = material;
}


void Editor::RenderComponent(std::string name,Component* component) {
	if (name == "Script") {
		Script* script = reinterpret_cast<Script*>(component);
		ImGui::Text("Script Name: Counter");
	}
	ImGui::Separator();
}