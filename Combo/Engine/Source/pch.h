//Copyright (c) 2022-2023 tecnessino. All rights reserved.

#include <iostream>
#include <vector>
#include <string>
#include <any>

#include <glm.hpp>

#define LOG(...) std::cout<<"\u001b[31m[COMBO]\u001b[0m "<<__VA_ARGS__<<std::endl;
#define CB_CRASH(...) std::cout<<"\u001b[31m[COMBO]\u001b[0m CRASH: "<<__VA_ARGS__<<std::endl; exit(0);
//#define CB_CHECK_API_ISNT_NULL(...) __VA_ARGS__ ? 
#define CB_ASSERT(cond,message,...) if(cond) {} else { LOG("Assertion Failed: " + std::string(message)) __debugbreak();}

#ifdef COMBO_BUILD_WINAPI
#include <Windows.h>
#include <windowsx.h>
#endif

#ifdef COMBO_DIRECTX11
#define CB_CHECKHR(...) if(__VA_ARGS__ == 0) {} else { LOG("DX Operation Failed: " + std::system_category().message(__VA_ARGS__)) __debugbreak(); }
#include <d3d11.h>
#include <SimpleMath.h>
#include <d3dcompiler.h>
using namespace DirectX::SimpleMath;
using namespace DirectX;
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib,"d3dcompiler.lib")
#endif