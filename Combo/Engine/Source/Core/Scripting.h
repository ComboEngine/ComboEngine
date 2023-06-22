#pragma once
#include "pch.h"
#include "Scope.h"
#include "Event.h"

class Scripting {
public:
	Event CSBeginPlay;
	Event CSUpdate;
	Event CSDraw;
	Event CSExit;
	std::vector<std::string> UpdateScripts;
	std::vector<std::string> ScriptNames;
	static void Create(Scripting** Obj);
};
