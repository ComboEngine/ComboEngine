#include "pch.h"
#include "../Core.h"
#include "SceneSerializer.h"
#include <nlohmann.h>
#include <fstream>

#include "../Renderer.h"

void SceneSerializer::LoadProject(std::string name)
{
}

nlohmann::json MarshalVector3(glm::vec3 vec) {
	nlohmann::json j;
	j["X"] = vec.x;
	j["Y"] = vec.y;
	j["Z"] = vec.z;
	return j;
}

void SceneSerializer::CreateProject(std::string name)
{
	nlohmann::json j;
	j["Name"] = name;

	std::vector<nlohmann::json> assets;

	for (const auto& a : Core::s_Project.Assets) {
		nlohmann::json asset;
		asset["UUID"] = a.first;
		asset["Type"] = a.second->GetType();
		asset["OriginalPath"] = a.second->path;
		asset["Name"] = a.second->GetName();

		assets.push_back(asset);
	}

	std::string sceneUUID = uuid::generate_uuid_v4();
	nlohmann::json scene;
	scene["UUID"] = sceneUUID;
	scene["Name"] = "Main";
	std::vector<nlohmann::json> actors;
	for (Actor* actor : Core::Actors) {
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
					componentJson["Mesh"] = renderer->mesh->uuid;
				}
				else {
					componentJson["Mesh"] = "0";
				}

				if (renderer->material != nullptr) {
					componentJson["Material"] = renderer->material->uuid;
				}
				else {
					componentJson["Material"] = "0";
				}

				std::map<std::string, std::string> perSubmesh;
				if (renderer->mesh != nullptr) {
					int index = 0;
					for (Submesh* submesh : std::any_cast<Mesh*>(renderer->mesh->GetHandle())->Submeshes) {
						if (submesh->Material != nullptr) {
							perSubmesh[std::to_string(index)] = submesh->Material->GetUUID();
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
			components.push_back(componentJson);
		}
		actorJson["Components"] = components;

		actors.push_back(actorJson);
	}
	scene["Actors"] = actors;


	std::vector<nlohmann::json> scenes = {
		scene
	};
	j["Assets"] = assets;
	j["Scenes"] = scenes;
	j["MainScene"] = sceneUUID;

	std::ofstream file(name + ".cbproject");
	file << j.dump(4);
	file.close();
}
