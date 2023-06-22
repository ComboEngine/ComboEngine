#include "pch.h"
#ifdef COMBO_BUILD_WIN32
#include <Core/Core.h>


int main(int argc, char** argv) {
	LOG("Running on Win32 Arch");
	HINSTANCE instance = GetModuleHandle(NULL);
	Platform::Create(&Core::s_Platform, instance);
	return Core::Init();
}
#endif