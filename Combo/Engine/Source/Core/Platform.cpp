#include "pch.h"
#include "Platform.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/PlatformDX11.h>
#endif

void Platform::Create(Scope<Platform>& platform,std::any PlatformData)
{
	platform = Scope<Platform>();
	//Select window api
#ifdef COMBO_DIRECTX11
	platform.Set(new PlatformDX11());
#endif
	platform.Get()->Init(PlatformData);

	LOG(std::string("Created ") + platform.Get()->GetApiName() + " Platform");
}
