#include "pch.h"
#include "Editor.h"
#include <Core/Core.h>
#include <Core/Mesh.h>
#include <Core/Renderer.h>
#include <Core/Input.h>
#include <Core/Script.h>
#include <Core/Camera.h>
#include <Core/Assets/ProjectSerializer.h>
#include "Color.h"

#include "Panels/ViewportPanel.h"
#include "Panels/ContentPanel.h"
#include "Panels/ActorPropertiesPanel.h"
#include "Panels/ScenePanel.h"
#include "Panels/MaterialPropertiesPanel.h"

#include <discord_rpc.h>
#pragma comment(lib,"DiscordRPC.lib")

std::vector<Panel*> Editor::Panels;
glm::vec3 Editor::LightDir;


void Editor::Init()
{
	const char* filters[] = { "*.cbproject" };
	const char* selection = tinyfd_openFileDialog("Select project", "./",1,filters, NULL, 0);
	ProjectSerializer::LoadProject(selection);
	Core::s_Window->SetTitle("Combo Editor 0.1 <" + Core::s_Context->GetApiName() + "> " + selection);

	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize("1122095911850479667", &Handle, 1, NULL);

	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = selection;
	discordPresence.details = "Editor 0.1";
	discordPresence.startTimestamp = 0;
	discordPresence.largeImageKey = "icon";
	discordPresence.largeImageText = "Combo Editor 0.1";
	discordPresence.smallImageKey = "icon";
	Discord_UpdatePresence(&discordPresence);

	Panels.push_back(new ViewportPanel());
	Panels.push_back(new ActorPropertiesPanel());
	Panels.push_back(new ContentPanel());
	Panels.push_back(new MaterialPropertiesPanel());
	Panels.push_back(new ScenePanel());
	Panels[3]->Show = false;

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
				if (ImGui::MenuItem("Position Gizmo")) { reinterpret_cast<ViewportPanel*>(Panels[0])->GizmoType = POSITION; }
				if (ImGui::MenuItem("Rotation Gizmo")) { reinterpret_cast<ViewportPanel*>(Panels[0])->GizmoType = ROTATION; }
				if (ImGui::MenuItem("Scale Gizmo")) { reinterpret_cast<ViewportPanel*>(Panels[0])->GizmoType = SCALE; }
				if (ImGui::MenuItem("All Gizmos")) { reinterpret_cast<ViewportPanel*>(Panels[0])->GizmoType = ALL; }
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
			Asset::ImportFromCb(&Obj, path,"");
		}
		else {
			if (fileExtension == ".fbx" || fileExtension == ".obj" || fileExtension == ".dae") {
				std::string name = std::filesystem::u8path(path).filename().string();
				Asset::ImportFromBinary(&Obj, path, name.substr(0, name.find_last_of(".")) + ".cbmesh","");
			}
		}

		if (fileExtension == ".cbmaterial") {
			Asset::ImportFromCb(&Obj, path,"");
		}

		if (fileExtension == ".cbtexture") {
			Asset::ImportFromCb(&Obj, path, "");
		}
		else {
			if (fileExtension == ".png" || fileExtension == ".jpg") {
				std::string name = std::filesystem::u8path(path).filename().string();
				Asset::ImportFromBinary(&Obj, path, name.substr(0, name.find_last_of(".")) + ".cbtexture", "");
			}
		}
	}
}
