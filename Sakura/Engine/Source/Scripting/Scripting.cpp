#include "Scripting.h"

sakura_array<sakura_ptr<Script>> Scripting::Scripts;

static int Log_Info(lua_State* L) // Lua callable functions must be this format
{
    const char* str = lua_tostring(L, 1); // get second param from stack
    Engine::logger.Info(str);
    return 0;
}

void Scripting::Init()
{
    //Sakura.Lua
    sakura_ptr<Script> test = make_shared<Script>();
    test->Create("Source/Sakura.lua");
    Scripts.push_back(test);
}

void Script::Create(std::string path)
{
    state = luaL_newstate();
    luaL_openlibs(state);
    lua_register(state, "info", Log_Info);
    luaL_dofile(state,path.c_str());
}

void Script::Update()
{
    lua_getglobal(state, "Update");
    lua_pcall(state, 0, 0, 0);
}
