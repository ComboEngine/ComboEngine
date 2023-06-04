#include "pch.h"
#include "Actor.h"

std::vector<std::shared_ptr<Script>> Actor::Scripts;

std::shared_ptr<Transform> Actor::GetTransform()
{
	return this->transform;
}

std::shared_ptr<Actor> Actor::Create()
{
	std::shared_ptr<Actor> actor = std::make_shared<Actor>();
	actor->transform = std::make_shared<Transform>();
	actor->transform->SetTransform(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	World::Actors.push_back(actor);
	return actor;
}
