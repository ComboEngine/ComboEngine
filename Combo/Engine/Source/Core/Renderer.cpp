#include "pch.h"
#include "Renderer.h"
#include "Core.h"

void Renderer::Init()
{
	Core::DrawEvent.Hook([&] {
		if (mesh.Get() != nullptr) {
			mesh.Get()->Render(Core::Render3DShader);
		}
	});
}

