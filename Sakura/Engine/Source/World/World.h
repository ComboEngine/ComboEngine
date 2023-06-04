#pragma once

#include "Actor.h"

class Script;
class Actor;
class World;


class World {
public:
	static std::vector<std::shared_ptr<Actor>> Actors;
	static void Init();
};