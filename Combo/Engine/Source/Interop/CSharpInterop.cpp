#include "pch.h"
#define COMBO_API_INTEROP _declspec(dllexport)

#include <Core/Core.h>
typedef void(__stdcall* BeginPlayCallback)();

extern "C" {
	COMBO_API_INTEROP void Platform_CSPreInit() {
		HINSTANCE instance = GetModuleHandle(NULL);
		Platform::Create(Core::s_Platform, instance);
	}

	COMBO_API_INTEROP int Core_Init() {
		return Core::Init();
	}

	COMBO_API_INTEROP void Interop_AddBeginPlayCallback(BeginPlayCallback callback) {
		Core::BeginPlayEvent.Hook([&] {
			callback();
		});
	}

	COMBO_API_INTEROP void Log_Info(const char* message) {
		LOG(message);
	}
}