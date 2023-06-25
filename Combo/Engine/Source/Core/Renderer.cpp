#include "pch.h"
#include "Renderer.h"
#include "Core.h"
#include "GlobalShaders.h"


void Renderer::Update(Actor* actor)
{
}

void Renderer::Draw(Actor* actor)
{
	if (mesh != nullptr && material != nullptr) {
		std::any_cast<Mesh*>(mesh->GetHandle())->Render(std::any_cast<Material*>(material->GetHandle()), actor->Position, glm::quat(glm::vec3(glm::radians(actor->Orientation.x), glm::radians(actor->Orientation.y), glm::radians(actor->Orientation.z))), actor->Scale);
	}
}

Component* Renderer::Clone()
{
	return new Renderer(*this);
}
