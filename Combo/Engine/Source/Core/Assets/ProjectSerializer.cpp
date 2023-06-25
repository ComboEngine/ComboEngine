#include "pch.h"
#include "../Core.h"
#include "ProjectSerializer.h"
#include "../Camera.h"
#include "../Renderer.h"
#include "../Lights/DirectionalLight.h"
#include "../Lights/PointLight.h"

nlohmann::json MarshalVector3(glm::vec3 vec) {
	nlohmann::json j;
	j["X"] = vec.x;
	j["Y"] = vec.y;
	j["Z"] = vec.z;
	return j;
}

glm::vec3 DemarshalVector3(nlohmann::json j) {
	return glm::vec3(j["X"], j["Y"], j["Z"]);
}

void ProjectSerializer::LoadProject(std::string path)
{
	Core::s_Project.ProjectSpaceDirectory = std::filesystem::path(path).parent_path().string() + "\\";

	Core::s_Project.Assets.clear();
	Core::Scene.Actors.clear();
	std::ifstream t(path.c_str());
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	nlohmann::json j = nlohmann::json::parse(str);

	for (nlohmann::json assetJson : j["Assets"]) {
		Asset* asset;
		Asset::ImportFromCb(&asset, Core::s_Project.ProjectSpaceDirectory + (std::string)assetJson["ProjectPath"],"");
		asset->Name = assetJson["Name"];
		Core::s_Project.Assets[asset->UUID] = asset;
 	}

	for (nlohmann::json sceneJson : j["Scenes"]) {
		for (nlohmann::json actorJson : sceneJson["Actors"]) {
			Actor* actor;
			Actor::Create(&actor);

			actor->UUID = actorJson["UUID"];
			actor->Name = actorJson["Name"];
			actor->Position = DemarshalVector3(actorJson["Position"]);
			actor->Orientation = DemarshalVector3(actorJson["Orientation"]);
			actor->Scale = DemarshalVector3(actorJson["Scale"]);

			for (nlohmann::json componentJson : actorJson["Components"]) {
				if (componentJson["Name"] == "Renderer") {
					Renderer* renderer = new Renderer();
					renderer->mesh = Core::s_Project.Assets[componentJson["Mesh"]];
					renderer->material = Core::s_Project.Assets[componentJson["Material"]];

					if (renderer->mesh != nullptr) {
						Mesh* mesh = std::any_cast<Mesh*>(renderer->mesh->GetHandle());
						for (int i = 0; i < mesh->Submeshes.size(); i++) {
							mesh->Submeshes[i]->Material = Core::s_Project.Assets[componentJson["MaterialsPerSubmesh"][std::to_string(i)]];
						}
					}
					actor->AddComponent(renderer);
				}
				else if (componentJson["Name"] == "Script") {

				}
				else if (componentJson["Name"] == "Directional Light") {
					DirectionalLight* light = new DirectionalLight();
					light->Intensity = componentJson["Intensity"];
					light->Color = DemarshalVector3(componentJson["Color"]);
					actor->AddComponent(light);
				}
				else if (componentJson["Name"] == "Point Light") {
					PointLight* light = new PointLight();
					light->Intensity = componentJson["Intensity"];
					light->Color = DemarshalVector3(componentJson["Color"]);
					actor->AddComponent(light);
				}
			}
		}
	}
}

void ProjectSerializer::CreateProject(std::string path)
{
	nlohmann::json j;
	std::string name = std::filesystem::u8path(path).filename().string();
	j["Name"] = name.substr(0, name.find_last_of("."));

	std::vector<nlohmann::json> assets;

	for (const auto& a : Core::s_Project.Assets) {
		if (a.first != "0") {
			nlohmann::json asset;
			asset["UUID"] = a.first;
			asset["Type"] = a.second->GetType();
			asset["ProjectPath"] = a.second->ProjectPath;
			asset["Name"] = a.second->Name;

			assets.push_back(asset);
		}
	}

	std::string sceneUUID = uuid::generate_uuid_v4();
	nlohmann::json scene;
	scene["UUID"] = sceneUUID;
	scene["Name"] = "Main";
	std::vector<nlohmann::json> actors;
	for (Actor* actor : Core::Scene.Actors) {
		nlohmann::json actorJson;
		actorJson["Name"] = actor->Name;
		actorJson["UUID"] = actor->UUID;
		actorJson["Position"] = MarshalVector3(actor->Position);
		actorJson["Orientation"] = MarshalVector3(actor->Orientation);
		actorJson["Scale"] = MarshalVector3(actor->Scale);
		std::vector<nlohmann::json> components;
		for (Component* component : actor->Components) {
			nlohmann::json componentJson;
			componentJson["Name"] = component->GetName();
			if (component->GetName() == "Renderer") {
				Renderer* renderer = reinterpret_cast<Renderer*>(component);
				if (renderer->mesh != nullptr) {
					componentJson["Mesh"] = renderer->mesh->UUID;
				}
				else {
					componentJson["Mesh"] = "0";
				}

				if (renderer->material != nullptr) {
					componentJson["Material"] = renderer->material->UUID;
				}
				else {
					componentJson["Material"] = "0";
				}

				std::map<std::string, std::string> perSubmesh;
				if (renderer->mesh != nullptr) {
					int index = 0;
					for (Submesh* submesh : std::any_cast<Mesh*>(renderer->mesh->GetHandle())->Submeshes) {
						if (submesh->Material != nullptr) {
							perSubmesh[std::to_string(index)] = submesh->Material->UUID;
						}
						else {
							perSubmesh[std::to_string(index)] = "0";
						}
						index++;
					}
				}
				componentJson["MaterialsPerSubmesh"] = perSubmesh;
			}
			if (component->GetName() == "Script") {

			}
			if (component->GetName() == "Directional Light") {
				DirectionalLight* light = reinterpret_cast<DirectionalLight*>(component);
				componentJson["Intensity"] = light->Intensity;
				componentJson["Color"] = MarshalVector3(light->Color);
			}
			if (component->GetName() == "Point Light") {
				PointLight* light = reinterpret_cast<PointLight*>(component);
				componentJson["Intensity"] = light->Intensity;
				componentJson["Color"] = MarshalVector3(light->Color);
			}
			components.push_back(componentJson);
		}
		actorJson["Components"] = components;

		actors.push_back(actorJson);
	}
	scene["Actors"] = actors;
	scene["EditorCameraPosition"] = MarshalVector3(Camera::Position);


	std::vector<nlohmann::json> scenes = {
		scene
	};
	j["Assets"] = assets;
	j["Scenes"] = scenes;
	j["MainScene"] = sceneUUID;

	std::ofstream file(path);
	file << j.dump(4);
	file.close();

	Core::s_Project.ProjectSpaceDirectory = std::filesystem::path(path).parent_path().string() + "\\";
}
