#pragma once

#include "Actor.h"

class Script;
class Actor;
class World;


class World {
public:
	static sakura_array<sakura_ptr<Actor>> Actors;
	static void Init();
};