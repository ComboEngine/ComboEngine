#include "pch.h"
#include "Renderer.h"
#include "Core.h"
#include "GlobalShaders.h"


void Renderer::Update(Scope<Actor> actor)
{
}

void Renderer::Draw(Scope<Actor> actor)
{
	if (mesh.Get() != nullptr) {
		mesh.Get()->Render(material, actor.Get()->Position, actor.Get()->Orientation, actor.Get()->Scale);
	}
}
