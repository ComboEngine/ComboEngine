project "Engine"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"
    runtime "Debug"
    cppdialect "C++17"

    targetdir "Output"
    objdir "Output/tmp"

    pchheader "pch.h"
    pchsource "Source/pch.cpp"

    files
	{
		"Source/**.h",
		"Source/**.cpp",
        "Source/**.hpp",
	}

    flags
	{
		"MultiProcessorCompile"
	}

    config = json.decode(io.readfile("Settings.json"))

    filter "system:windows"
        defines {
            "COMBO_BUILD_WIN32",
            "COMBO_BUILD_WINAPI"
        }

    if(config["GraphicsAPI"] == "DirectX11") then
        includedirs {
            "Source",
            "../Thirdparty/GLM/Source",
            "../Thirdparty/SmallLibraries/Source",
            "../Thirdparty/LuaC/Source",
            "../Thirdparty/Assimp/Source",
            "../Thirdparty/Optick/Source",
            "../Thirdparty/Discord/Source",
            "../Thirdparty/ImGui/Source",
            "../Thirdparty/GLFW/Source",
            "../Thirdparty/DirectXTK/Source"
        }
    
        libdirs {
            "../Thirdparty/GLM/Library",
            "../Thirdparty/SmallLibraries/Library",
            "../Thirdparty/LuaC/Library",
            "../Thirdparty/Assimp/Library",
            "../Thirdparty/Optick/Library",
            "../Thirdparty/Discord/Library",
            "../Thirdparty/ImGui/Library",
            "../Thirdparty/GLFW/Library",
            "../Thirdparty/DirectXTK/Library"
        }
    
        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "COMBO_DIRECTX11"
        }


    else
        includedirs {
            "Source",
            "../Thirdparty/GLM/Source",
            "../Thirdparty/SmallLibraries/Source",
            "../Thirdparty/LuaC/Source",
            "../Thirdparty/Assimp/Source",
            "../Thirdparty/Optick/Source",
            "../Thirdparty/Discord/Source",
            "../Thirdparty/ImGui/Source",
            "../Thirdparty/GLFW/Source",
            "../Thirdparty/Vulkan/Source"
        }
    
        libdirs {
            "../Thirdparty/GLM/Library",
            "../Thirdparty/SmallLibraries/Library",
            "../Thirdparty/LuaC/Library",
            "../Thirdparty/Assimp/Library",
            "../Thirdparty/Optick/Library",
            "../Thirdparty/Discord/Library",
            "../Thirdparty/ImGui/Library",
            "../Thirdparty/GLFW/Library",
            "../Thirdparty/Vulkan/Library"
        }
    
        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "COMBO_VULKAN"
        }
    end