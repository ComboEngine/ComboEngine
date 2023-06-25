#include "pch.h"
#include "Core.h"
#include "Actor.h"

void Actor::Create(Actor** actor)
{
	*actor = new Actor();
	Actor* ptr = *actor;
	ptr->UUID = uuid::generate_uuid_v4();
	Core::Scene.Actors.push_back(*actor);
}

Actor* Actor::Clone()
{
	Actor* actorDuplicated = new Actor();
	actorDuplicated->UUID = uuid::generate_uuid_v4();
	actorDuplicated->Name = this->Name + " Copy";
	actorDuplicated->Position = this->Position;
	actorDuplicated->Orientation = this->Orientation;
	actorDuplicated->Scale = this->Scale;
	for (Component* component : this->Components) {
		actorDuplicated->AddComponent(component->Clone());
	}
	Core::Scene.Actors.push_back(actorDuplicated);
	return actorDuplicated;
}

void Actor::AddComponent(Component* component)
{
	Components.push_back(component);
}
