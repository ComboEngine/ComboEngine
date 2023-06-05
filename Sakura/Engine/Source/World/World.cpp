#include "pch.h"
#include "World.h"
#include "Camera.h"

std::vector<std::shared_ptr<Actor>> World::Actors;
std::shared_ptr<Camera> World::camera;


std::shared_ptr<Camera> World::GetCamera()
{
	return camera;
}

void World::Init()
{
	camera = std::make_shared<Camera>();
	camera->transform = std::make_shared<Transform>();

}

void World::Update()
{
	GetCamera()->UpdateCamera();
}
