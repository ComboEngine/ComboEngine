#include "pch.h"
#include "Platform.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/PlatformDX11.h>
#endif
#ifdef COMBO_VULKAN
#include <Core/Vulkan/PlatformVulkan.h>
#endif

void Platform::Create(Platform** platform,std::any PlatformData)
{
	//Select window api
#ifdef COMBO_DIRECTX11
	*platform = new PlatformDX11();
#endif
#ifdef COMBO_VULKAN
	*platform = new PlatformVulkan();
#endif
	if (*platform != nullptr) {
		Platform* platformPtr = *platform;
		platformPtr->Init(PlatformData);

		LOG(std::string("Created ") + platformPtr->GetApiName() + " Platform");
	}
}
