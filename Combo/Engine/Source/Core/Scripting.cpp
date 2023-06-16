#include "pch.h"
#include "Scripting.h"
#include "Core.h"



int Scripting::LogInfo(lua_State* L)
{
	const char* str = lua_tostring(L, 1);
	LOG(str);
	return 0;
}

void Scripting::Create(Scope<Scripting>& Obj)
{
	Scope<Scripting>::Create(Obj);

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_register(L, "Log", Scripting::LogInfo);
	luaL_dofile(L,"Main.lua");

	lua_getglobal(L, "Update");
	lua_pcallk(L, (0), (0), (0), 0, 0);

	Core::UpdateEvent.Hook([&] {

	});
}