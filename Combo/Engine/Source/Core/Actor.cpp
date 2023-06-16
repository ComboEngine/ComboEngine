#include "pch.h"
#include "Core.h"
#include "Actor.h"

void Actor::Create(Scope<Actor>& Obj)
{
	Scope<Actor>::Create(Obj);
	Core::Actors.push_back(Obj);
}

void Actor::AddComponent(Component* component)
{
	Components.push_back(component);
}
