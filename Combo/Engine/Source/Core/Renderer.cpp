#include "pch.h"
#include "Renderer.h"
#include "Core.h"
#include "GlobalShaders.h"


void Renderer::Update(Scope<Actor> actor)
{
}

void Renderer::Draw(Scope<Actor> actor)
{
	if (mesh != nullptr) {
		std::any_cast<Scope<Mesh>>(mesh->GetHandle()).Get()->Render(material, actor.Get()->Position, glm::quat(glm::vec3(glm::radians(actor.Get()->Orientation.x), glm::radians(actor.Get()->Orientation.y), glm::radians(actor.Get()->Orientation.z))) , actor.Get()->Scale);
	}
}
