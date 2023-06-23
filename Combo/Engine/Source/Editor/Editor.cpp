#include "pch.h"
#include "Editor.h"
#include <Core/Core.h>
#include <Core/Mesh.h>
#include <Core/Renderer.h>
#include <Core/Input.h>
#include <Core/Script.h>
#include <Core/Camera.h>
#include <Core/Assets/MeshAsset.h>
#include <Core/Assets/ProjectSerializer.h>
#include "Color.h"

#include "Panels/ViewportPanel.h"
#include "Panels/ContentPanel.h"
#include "Panels/ActorPropertiesPanel.h"
#include "Panels/MaterialPropertiesPanel.h"

std::vector<Panel*> Editor::Panels;
glm::vec3 Editor::LightDir;


void Editor::Init()
{
	const char* filters[] = { "*.cbproject" };
	const char* selection = tinyfd_openFileDialog("Select project", "./",1,filters, NULL, 0);
	ProjectSerializer::LoadProject(selection);
	Core::s_Window->SetTitle("Combo Editor 0.1 <" + Core::s_Context->GetApiName() + "> " + selection);

	Panels.push_back(new ViewportPanel());
	Panels.push_back(new ActorPropertiesPanel());
	Panels.push_back(new ContentPanel());
	Panels.push_back(new MaterialPropertiesPanel());
	Panels[3]->Show = false;

	ImGui::GetIO().Fonts->AddFontFromFileTTF("./Content/Roboto.ttf", 15.0f);
	Colours::Set();

	Core::DrawEvent.Hook([&] {
		if (Input::IsKeyDown(COMBO_KEY_ESCAPE)) {
			reinterpret_cast<ViewportPanel*>(Panels[0])->MouseHooked = false;
		}
		Core::s_Window->LockCursor(reinterpret_cast<ViewportPanel*>(Panels[0])->MouseHooked);
		if (reinterpret_cast<ViewportPanel*>(Panels[0])->MouseHooked) {
			Camera::Drone();
		}
	});
	Core::ImGuiDrawEvent.Hook([&] {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::Begin("Dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save Project"))
				{
					const char* filters[] = { "*.cbproject" };
					ProjectSerializer::CreateProject(tinyfd_saveFileDialog("Save Project", "./", 1, filters, NULL));
				}
				if(ImGui::MenuItem("Create Actor")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Viewport")) {
				if (ImGui::MenuItem("Position")) { reinterpret_cast<ViewportPanel*>(Panels[0])->ViewMode = Position; }
				if (ImGui::MenuItem("Normal")) { reinterpret_cast<ViewportPanel*>(Panels[0])->ViewMode = Normal; }
				if (ImGui::MenuItem("Diffuse")) { reinterpret_cast<ViewportPanel*>(Panels[0])->ViewMode = Diffuse; }
				if (ImGui::MenuItem("GBuffer")) { reinterpret_cast<ViewportPanel*>(Panels[0])->ViewMode = FinalBuffer; }
				ImGui::End();
			}

			if (ImGui::BeginMenu("Windows")) {
				for (Panel* panel : Panels) {
					ImGui::MenuItem(panel->GetName().c_str(), NULL, &panel->Show);
				}
				ImGui::End();
			}

			ImGui::EndMainMenuBar();
		}
		ImGui::SetCursorPosY(40);
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));

		for (Panel* panel : Panels) {
			if (panel->Show) {
				ImGui::Begin(panel->GetName().c_str());
				panel->Draw();
				ImGui::End();
			}
		}

		ImGui::Begin("Graphics Settings");
		ImGui::End();

		ImGui::End();
	});
}


void Editor::OnDrop(std::vector<std::string> paths)
{
	Asset* Obj;
	for (std::string path : paths) {
		std::string fileExtension = std::filesystem::u8path(path).extension().string();

		if (fileExtension == ".cbproject") {
			Core::s_Window->SetTitle("Combo Editor 0.1 <" + Core::s_Context->GetApiName() + "> " + path);
			ProjectSerializer::LoadProject(path);
		}

		if (fileExtension == ".cbmesh") {
			Asset::Create(&Obj, path);
		}
		else {
			if (fileExtension == ".fbx" || fileExtension == ".obj") {
				std::string name = std::filesystem::u8path(path).filename().string();
				Asset::Import(&Obj, path, std::string("./") + name.substr(0, name.find_last_of(".")) + ".cbmesh", NULL);
			}
		}

		if (fileExtension == ".cbmaterial") {
			Asset::Create(&Obj, path);
		}
	}
}
