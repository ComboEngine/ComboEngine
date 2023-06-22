#include "pch.h"
#include "Platform.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/PlatformDX11.h>
#endif

void Platform::Create(Platform** platform,std::any PlatformData)
{
	//Select window api
#ifdef COMBO_DIRECTX11
	*platform = new PlatformDX11();
#endif
	Platform* platformPtr = *platform;
	platformPtr->Init(PlatformData);

	LOG(std::string("Created ") + platformPtr->GetApiName() + " Platform");
}
