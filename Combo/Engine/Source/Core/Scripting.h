#pragma once
#include "pch.h"
#include "Scope.h"

class Scripting {
public:
	static int LogInfo(lua_State* L);

	static void Create(Scope<Scripting>& Obj);
};

