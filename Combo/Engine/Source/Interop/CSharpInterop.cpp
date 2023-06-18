#include "pch.h"
#define COMBO_API_INTEROP _declspec(dllexport)

#include <Core/Core.h>


typedef void(__stdcall* BeginPlayCallback)();
typedef void(__stdcall* UpdateCallback)();
typedef void(__stdcall* DrawCallback)();
typedef void(__stdcall* ExitCallback)();

extern "C" {
	COMBO_API_INTEROP void Platform_CSPreInit() {
		HINSTANCE instance = GetModuleHandle(NULL);
		Platform::Create(Core::s_Platform, instance);
	}

	COMBO_API_INTEROP int Core_Init() {
		return Core::Init();
	}

	COMBO_API_INTEROP void Interop_AddBeginPlayCallback(BeginPlayCallback callback) {
		Core::BeginPlayEvent.Hook([=] {
			callback();
		});
	}
	COMBO_API_INTEROP void Interop_AddUpdateCallback(UpdateCallback callback) {
		Core::UpdateEvent.Hook([=] {
			callback();
		});
	}
	COMBO_API_INTEROP void Interop_AddDrawCallback(DrawCallback callback) {
		Core::DrawEvent.Hook([=] {
			callback();
		});
	}
	COMBO_API_INTEROP void Interop_AddExitCallback(ExitCallback callback) {
		Core::ExitEvent.Hook([=] {
			callback();
		});
	}

	COMBO_API_INTEROP void Log_Info(const char* message) {
		LOG(message);
	}
	COMBO_API_INTEROP void Interop_ExposeComponents(const char* components[], int arraySize) {
		std::vector<std::string> Names;
		for (int i = 0; i < arraySize;i++) {
			Names.push_back(components[i]);
		}
		Core::s_Scripting.Get()->ScriptNames = Names;
	}
}