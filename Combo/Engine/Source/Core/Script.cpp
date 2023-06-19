#include "pch.h"
#include "Script.h"
#include "Core.h"
#include "GlobalShaders.h"


void Script::Update(Scope<Actor> actor)
{
	Core::s_Scripting.Get()->UpdateScripts.push_back(ComponentName);
}

void Script::Draw(Scope<Actor> actor)
{
	
}
