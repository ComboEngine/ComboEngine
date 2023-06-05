#pragma once
#include "Actor.h"
#include "Camera.h"

class Script;
class Actor;
class World;
class Camera;


class World {
public:
	static std::shared_ptr<Camera> camera;
public:
	static std::shared_ptr<Camera> GetCamera();
	static std::vector<std::shared_ptr<Actor>> Actors;
	static void Init();
	static void Update();
};