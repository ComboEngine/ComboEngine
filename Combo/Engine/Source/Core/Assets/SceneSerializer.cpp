#include "pch.h"
#include "SceneSerializer.h"

#include <Core/Core.h>
#include <Core/Mesh.h>
#include <Core/Renderer.h>
#include <Core/Assets/TextureSerializer.h>
#include <Core/Assets/MeshSerializer.h>
#include <Core/Camera.h>
#include <Core/Input.h>

void SceneSerializer::Load(std::string path)
{
	bool mouseSwitch;
	Core::ImGuiDrawEvent.Hook([&] {
		ImGui::Begin("xd");
		if (Input::IsKeyDown(COMBO_KEY_ESCAPE)) {
			mouseSwitch = !mouseSwitch;
		}
		Core::s_Window.Get()->LockCursor(mouseSwitch);
		if (mouseSwitch) {
			Camera::Drone();
		}
		ImGui::Image((void*)Core::Framebuffers[RenderStage::COLOR].Get()->GetImage(), ImGui::GetWindowSize());
		ImGui::End();
	});

	//game code
	Scope<Actor> actor;
	Actor::Create(actor);

	actor.Get()->Scale = glm::vec3(0.001f, 0.001f, 0.001f);

	Scope<Renderer> renderer;
	Scope<Renderer>::Create(renderer);

	actor.Get()->AddComponent(renderer.Cast<Component>());

	Scope<Mesh> mesh;
	MeshSerializer::Read(mesh, "Sponza.cbmesh");

	renderer.Get()->mesh = mesh;

	Scope<Texture> test;
	TextureSerializer::Read(test, "test.cbtexture");

	Scope<Material> material;
	Material::Create(material);
	material.Get()->Diffuse = MaterialColor::FromColor(glm::vec4(1, 1, 1, 1));

	renderer.Get()->material = material;
	//end game code
}
