#include "pch.h"
#include "Editor.h"
#include <Core/Core.h>
#include <Core/Mesh.h>
#include <Core/Renderer.h>
#include <Core/Input.h>
#include <Core/Render2D.h>
#include <Core/Camera.h>
#include <Core/Script.h>
#include <Core/Assets/MeshAsset.h>
#include <imgui_stdlib.h>
#include <Core/Assets/SceneSerializer.h>
#include "Color.h"

bool Editor::MouseHooked;
std::string Editor::ToImportPathBuffer;
std::string Editor::AssetPathBuffer;
bool Editor::ShowImport = false;
bool Editor::ShowImportExisting = false;

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
		Core::s_Window->LockCursor(MouseHooked);
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
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save Project"))
				{
					SceneSerializer::CreateProject("test");
				}
				if (ImGui::MenuItem("Import"))
				{
					ShowImport = true;
				}
				if (ImGui::MenuItem("Import Existing"))
				{
					ShowImportExisting = true;
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
		ImGui::SetCursorPosY(40);
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));

		ImGui::Begin("Viewport");
		Camera::ProjectionWidth = ImGui::GetContentRegionAvail().x;
		Camera::ProjectionHeight = ImGui::GetContentRegionAvail().y;
		if (ImGui::ImageButton((void*)Core::s_Final->GetImage(), ImGui::GetContentRegionAvail())) {
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
			Actor* actor = Core::Actors[i];
			ImGui::TableNextColumn();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4);
			ImGui::Selectable((std::string(actor->Name.c_str()) + "##" + std::to_string(i)).c_str(), &actor->ActorSelected, ImGuiSelectableFlags_SpanAllColumns);
		}

		ImGui::EndTable();

		ImGui::End();

		ImGui::Begin("Actor Properties");

		for (int i = 0; i < Core::Actors.size(); i++) {
			Actor* actor = Core::Actors[i];
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
						RenderComponent(name,component);
					}
				}

				ImGui::Separator();
			}
		}
		ImGui::End();

		ImGui::Begin("Content");
		int index = 0;
		for (const auto& asset : Core::s_Project.Assets) {
			if (asset.second != nullptr) {
				ImGui::SameLine();
				ImGui::Button((asset.second->GetName() + "##" + std::to_string(index)).c_str(),ImVec2(128,128));

				if (ImGui::BeginDragDropSource()) {
					ImGui::SetDragDropPayload(asset.second->GetType().c_str(), asset.first.c_str(), sizeof(asset.first));
					ImGui::Text(asset.first.c_str());
					ImGui::EndDragDropSource();
				}
			}
			index++;
		}

		ImGui::End();

		ImGui::Begin("Graphics Settings");
		//ImGui::InputFloat4("Light Pos", glm::value_ptr(Core::LightPos));
		//ImGui::InputFloat("Light Range", &Core::LightRange);
		//ImGui::InputFloat4("Light Atteunation", glm::value_ptr(Core::LightAttenuation));
		ImGui::End();

		if (ShowImport) {
			ImGui::Begin("Import");

			ImGui::Text("To Import Path");
			ImGui::SameLine();
			ImGui::InputText("##ToImportPath", &ToImportPathBuffer);
			ImGui::Separator();

			ImGui::Text("Asset Path");
			ImGui::SameLine();
			ImGui::InputText("##AssetPath", &AssetPathBuffer);
			ImGui::Separator();

			if (ImGui::Button("Import")) {
				ImportAsset();
				ShowImport = false;
				ToImportPathBuffer = "";
				AssetPathBuffer = "";
			}
			ImGui::End();
		}

		if (ShowImportExisting) {
			ImGui::Begin("Import Existing");

			ImGui::Text("Asset Path");
			ImGui::SameLine();
			ImGui::InputText("##AssetPath", &AssetPathBuffer);
			ImGui::Separator();

			if (ImGui::Button("Import")) {
				ImportExistingAsset();
				ShowImportExisting = false;
				ToImportPathBuffer = "";
				AssetPathBuffer = "";
			}
			ImGui::End();
		}

		ImGui::End();
	});

	Actor* actor;
	Actor::Create(&actor);

	Actor* actor2;
	Actor::Create(&actor2);

	Renderer* renderer = new Renderer();

	Script* script = new Script();

	actor->AddComponent(renderer);
	actor->AddComponent(script);

	//Asset::Create(asset, "Sponza.cbmesh");


	//renderer.Get()->mesh = asset.Get();
}


void Editor::RenderComponent(std::string name,Component* component) {
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

void Editor::ImportAsset()
{
	Asset* Obj;
	std::string fileExtension = std::filesystem::u8path(ToImportPathBuffer).extension().string();
	if (fileExtension == ".fbx" || fileExtension == ".obj") {
		Asset::Import(&Obj, ToImportPathBuffer, AssetPathBuffer, NULL);
	}
}

void Editor::ImportExistingAsset()
{
	Asset::Create(nullptr, AssetPathBuffer);
}

void Editor::OnDrop(std::vector<std::string> paths)
{
	Asset* Obj;
	for (std::string path : paths) {
		std::string fileExtension = std::filesystem::u8path(path).extension().string();
		if (fileExtension == ".cbmesh") {
			Asset::Create(&Obj, path);
		}
		else {
			if (fileExtension == ".fbx" || fileExtension == ".obj") {
				//Right now we importing asset to project path
				std::string name = std::filesystem::u8path(path).filename().string();
				Asset::Import(&Obj, path, std::string("./") + name.substr(0, name.find_last_of(".")) + ".cbmesh", NULL);
			}
		}

		if (fileExtension == ".cbmaterial") {
			Asset::Create(&Obj, path);
		}
	}
}
