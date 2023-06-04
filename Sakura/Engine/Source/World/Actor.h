#pragma once

#include <pch.h>
#include <Scripting/Scripting.h>
#include <Math/Transform.h>

class Script;

class Actor {
private:
	std::shared_ptr<Transform> transform;
public:
	std::shared_ptr<Transform> GetTransform();
	std::string Name = "Actor";
	static std::vector<std::shared_ptr<Script>> Scripts;
	static std::shared_ptr<Actor> Create();
};