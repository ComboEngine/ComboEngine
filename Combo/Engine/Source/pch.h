//Copyright (c) 2022-2023 tecnessino. All rights reserved.

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <any>
#include <fstream>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/matrix_inverse.hpp>
#include <detail/type_quat.hpp>
#include <gtx/quaternion.hpp>


//Enable editor
#define COMBO_EDITOR

/* The unknown key */
#define COMBO_KEY_UNKNOWN            -1

/* Printable keys */
#define COMBO_KEY_SPACE              32
#define COMBO_KEY_APOSTROPHE         39  /* ' */
#define COMBO_KEY_COMMA              44  /* , */
#define COMBO_KEY_MINUS              45  /* - */
#define COMBO_KEY_PERIOD             46  /* . */
#define COMBO_KEY_SLASH              47  /* / */
#define COMBO_KEY_0                  48
#define COMBO_KEY_1                  49
#define COMBO_KEY_2                  50
#define COMBO_KEY_3                  51
#define COMBO_KEY_4                  52
#define COMBO_KEY_5                  53
#define COMBO_KEY_6                  54
#define COMBO_KEY_7                  55
#define COMBO_KEY_8                  56
#define COMBO_KEY_9                  57
#define COMBO_KEY_SEMICOLON          59  /* ; */
#define COMBO_KEY_EQUAL              61  /* = */
#define COMBO_KEY_A                  65
#define COMBO_KEY_B                  66
#define COMBO_KEY_C                  67
#define COMBO_KEY_D                  68
#define COMBO_KEY_E                  69
#define COMBO_KEY_F                  70
#define COMBO_KEY_G                  71
#define COMBO_KEY_H                  72
#define COMBO_KEY_I                  73
#define COMBO_KEY_J                  74
#define COMBO_KEY_K                  75
#define COMBO_KEY_L                  76
#define COMBO_KEY_M                  77
#define COMBO_KEY_N                  78
#define COMBO_KEY_O                  79
#define COMBO_KEY_P                  80
#define COMBO_KEY_Q                  81
#define COMBO_KEY_R                  82
#define COMBO_KEY_S                  83
#define COMBO_KEY_T                  84
#define COMBO_KEY_U                  85
#define COMBO_KEY_V                  86
#define COMBO_KEY_W                  87
#define COMBO_KEY_X                  88
#define COMBO_KEY_Y                  89
#define COMBO_KEY_Z                  90
#define COMBO_KEY_LEFT_BRACKET       91  /* [ */
#define COMBO_KEY_BACKSLASH          92  /* \ */
#define COMBO_KEY_RIGHT_BRACKET      93  /* ] */
#define COMBO_KEY_GRAVE_ACCENT       96  /* ` */
#define COMBO_KEY_WORLD_1            161 /* non-US #1 */
#define COMBO_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define COMBO_KEY_ESCAPE             256
#define COMBO_KEY_ENTER              257
#define COMBO_KEY_TAB                258
#define COMBO_KEY_BACKSPACE          259
#define COMBO_KEY_INSERT             260
#define COMBO_KEY_DELETE             261
#define COMBO_KEY_RIGHT              262
#define COMBO_KEY_LEFT               263
#define COMBO_KEY_DOWN               264
#define COMBO_KEY_UP                 265
#define COMBO_KEY_PAGE_UP            266
#define COMBO_KEY_PAGE_DOWN          267
#define COMBO_KEY_HOME               268
#define COMBO_KEY_END                269
#define COMBO_KEY_CAPS_LOCK          280
#define COMBO_KEY_SCROLL_LOCK        281
#define COMBO_KEY_NUM_LOCK           282
#define COMBO_KEY_PRINT_SCREEN       283
#define COMBO_KEY_PAUSE              284
#define COMBO_KEY_F1                 290
#define COMBO_KEY_F2                 291
#define COMBO_KEY_F3                 292
#define COMBO_KEY_F4                 293
#define COMBO_KEY_F5                 294
#define COMBO_KEY_F6                 295
#define COMBO_KEY_F7                 296
#define COMBO_KEY_F8                 297
#define COMBO_KEY_F9                 298
#define COMBO_KEY_F10                299
#define COMBO_KEY_F11                300
#define COMBO_KEY_F12                301
#define COMBO_KEY_F13                302
#define COMBO_KEY_F14                303
#define COMBO_KEY_F15                304
#define COMBO_KEY_F16                305
#define COMBO_KEY_F17                306
#define COMBO_KEY_F18                307
#define COMBO_KEY_F19                308
#define COMBO_KEY_F20                309
#define COMBO_KEY_F21                310
#define COMBO_KEY_F22                311
#define COMBO_KEY_F23                312
#define COMBO_KEY_F24                313
#define COMBO_KEY_F25                314
#define COMBO_KEY_KP_0               320
#define COMBO_KEY_KP_1               321
#define COMBO_KEY_KP_2               322
#define COMBO_KEY_KP_3               323
#define COMBO_KEY_KP_4               324
#define COMBO_KEY_KP_5               325
#define COMBO_KEY_KP_6               326
#define COMBO_KEY_KP_7               327
#define COMBO_KEY_KP_8               328
#define COMBO_KEY_KP_9               329
#define COMBO_KEY_KP_DECIMAL         330
#define COMBO_KEY_KP_DIVIDE          331
#define COMBO_KEY_KP_MULTIPLY        332
#define COMBO_KEY_KP_SUBTRACT        333
#define COMBO_KEY_KP_ADD             334
#define COMBO_KEY_KP_ENTER           335
#define COMBO_KEY_KP_EQUAL           336
#define COMBO_KEY_LEFT_SHIFT         340
#define COMBO_KEY_LEFT_CONTROL       341
#define COMBO_KEY_LEFT_ALT           342
#define COMBO_KEY_LEFT_SUPER         343
#define COMBO_KEY_RIGHT_SHIFT        344
#define COMBO_KEY_RIGHT_CONTROL      345
#define COMBO_KEY_RIGHT_ALT          346
#define COMBO_KEY_RIGHT_SUPER        347
#define COMBO_KEY_MENU               348

#define COMBO_KEY_LAST               COMBO_KEY_MENU

#define LOG(...) std::cout<<"\u001b[31m[COMBO]\u001b[0m "<<__VA_ARGS__<<std::endl;
#define CB_CRASH(...) std::cout<<"\u001b[31m[COMBO]\u001b[0m CRASH: "<<__VA_ARGS__<<std::endl; exit(0);
//#define CB_CHECK_API_ISNT_NULL(...) __VA_ARGS__ ? 
#define CB_ASSERT(cond,message,...) if(cond) {} else { LOG("Assertion Failed: " + std::string(message)) __debugbreak();}

#ifdef COMBO_BUILD_WINAPI
#include <Windows.h>
#include <windowsx.h>
#endif

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#pragma comment(lib,"lua54.lib")

#include <nlohmann.h>

#include <tinyfiledialogs.h>
#pragma comment(lib,"tinyfiledialogs64.lib")

#include <Importer.hpp>
#include <postprocessAi.h>
#include <scene.h>
#pragma comment(lib,"assimp-vc143-mtd.lib")

#include <optick.h>
#pragma comment(lib,"OptickCore.lib")

#include <imgui_impl_glfw.h>
#include <ImGuizmo.h>
#include <glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#pragma comment(lib,"ImGui.lib")

#ifdef COMBO_VULKAN
#define CB_CHECKHR(...) if(__VA_ARGS__ == VK_SUCCESS) {} else { LOG("Vulkan Operation Failed") __debugbreak(); }
#include <imgui_impl_vulkan.h>
#pragma comment(lib,"vulkan-1.lib")
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>
#pragma comment(lib,"glfw3.lib")

#ifdef COMBO_BUILD_WINAPI
#include <vulkan/vulkan_win32.h>
#endif

#endif

#ifdef COMBO_DIRECTX11
#define CB_CHECKHR(...) if(__VA_ARGS__ == 0) {} else { LOG("DX Operation Failed: " + std::system_category().message(__VA_ARGS__)) __debugbreak(); }
#include <d3d11.h>
#include <SimpleMath.h>
#include <d3dcompiler.h>
#include <imgui_impl_dx11.h>
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>
#pragma comment(lib,"glfw3.lib")
#include <imgui_impl_win32.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib,"d3dcompiler.lib")
#endif