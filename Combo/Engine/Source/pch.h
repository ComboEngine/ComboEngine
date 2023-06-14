//Copyright (c) 2022-2023 tecnessino. All rights reserved.

#include <iostream>
#include <vector>
#include <string>
#include <any>

#define LOG(...) std::cout<<"\u001b[31m[COMBO]\u001b[0m "<<__VA_ARGS__<<std::endl;

#ifdef COMBO_BUILD_WINAPI
#include <Windows.h>
#include <windowsx.h>
#endif

#ifdef COMBO_DIRECTX11
#endif