#include "pch.h"
#include "Script.h"
#include "Core.h"
#include "GlobalShaders.h"


void Script::Update(Actor* actor)
{
	Core::s_Scripting->UpdateScripts.push_back(ComponentName);
}

void Script::Draw(Actor* actor)
{
	
}
