#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/Platform.h>

class PlatformDX11 : public Platform {
public:
	HINSTANCE hInstance;
	virtual void Init(std::any PlatformData);
	virtual std::string GetApiName();
};
#endif