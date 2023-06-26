#ifdef COMBO_VULKAN
#pragma once
#include <Core/Platform.h>

class PlatformVulkan : public Platform {
public:
	virtual void Init(std::any PlatformData);
	virtual std::string GetApiName();
};
#endif