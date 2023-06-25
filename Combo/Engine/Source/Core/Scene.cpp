#include "pch.h"
#include "Scene.h"

void Scene::RemoveActor(Actor* actor)
{
	ActorDeleteQueue.push_back(actor);
}

void Scene::EndScene()
{
	for (Actor* actor : ActorDeleteQueue) {
		Actors.erase(std::remove(Actors.begin(), Actors.end(), actor), Actors.end());
		delete actor;
		ActorDeleteQueue.erase(std::remove(ActorDeleteQueue.begin(), ActorDeleteQueue.end(), actor), ActorDeleteQueue.end());
	}

	LightingData.clear();
}
