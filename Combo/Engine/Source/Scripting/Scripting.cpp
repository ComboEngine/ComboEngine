#include "pch.h"
#include "Scripting.h"

#include <Graphics/GPU2D.h>

std::vector<std::shared_ptr<Script>> Scripting::Scripts;

static int Log_Info(lua_State* L) // Lua callable functions must be this format
{
    const char* str = lua_tostring(L, 1); // get second param from stack
    Engine::logger.Info(str);
    return 0;
}

static int Render2D_RenderQuad(lua_State* L) {
    float x = lua_tointeger(L, 1);
    float y = lua_tointeger(L, 2);
    float width = lua_tointeger(L, 3);
    float height = lua_tointeger(L, 4);

    GPU2D::RenderQuad(x, y, width, height, Color32(1, 1, 1, 1), GPU::Instance->RenderPass);

    return 0;
}

void Scripting::Init()
{
    //Combo.Lua
    std::shared_ptr<Script> test = std::make_shared<Script>();
    test->Create("Source/Combo.lua");
    Scripts.push_back(test);
}

void Script::Create(std::string path)
{
    state = luaL_newstate();
    luaL_openlibs(state);
    lua_register(state, "info", Log_Info);
    lua_register(state, "RenderQuad", Render2D_RenderQuad);
    luaL_dofile(state, path.c_str());
}

void Script::Update()
{
    lua_getglobal(state, "Update");
    lua_pcall(state, 0, 0, 0);
}

void Script::Draw()
{
    lua_getglobal(state, "Draw");
    lua_pcall(state, 0, 0, 0);
}