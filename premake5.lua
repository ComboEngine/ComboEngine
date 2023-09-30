workspace "Combo"
    architecture "x86_64"
    startproject "Engine"

    configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	

group "Core"
    include "Combo/Engine"

group "Lib"
	include "Combo/Thirdparty/GLM"