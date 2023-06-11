#pragma once
#include <Engine/Engine.h>

#pragma comment(lib,"lua54.lib")

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

class Script {
private:
	lua_State* state;
public:
	void Create(std::string path);
	void Update();
	void Draw();
};


class Scripting {
public:
	static std::vector<std::shared_ptr<Script>> Scripts;
	static void Init();
};