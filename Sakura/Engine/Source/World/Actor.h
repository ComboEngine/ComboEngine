#pragma once

#include <Engine/EngineHeaders.h>
#include <Scripting/Scripting.h>

class Script;

class Actor {
public:
	std::string Name = "Actor";
	static sakura_array<sakura_ptr<Script>> Scripts;
};