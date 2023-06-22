#include "pch.h"
#include "Core.h"
#include "Actor.h"

void Actor::Create(Actor** actor)
{
	*actor = new Actor();
	Actor* ptr = *actor;
	ptr->UUID = uuid::generate_uuid_v4();
	Core::Actors.push_back(*actor);
}

void Actor::AddComponent(Component* component)
{
	Components.push_back(component);
}
