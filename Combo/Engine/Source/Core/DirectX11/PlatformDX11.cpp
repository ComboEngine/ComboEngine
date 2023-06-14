#include "pch.h"
#ifdef COMBO_DIRECTX11
#include "PlatformDX11.h"


//Now PlatformData is just a HINSTANCE
void PlatformDX11::Init(std::any PlatformData)
{
	this->hInstance = std::any_cast<HINSTANCE>(PlatformData);
}
std::string PlatformDX11::GetApiName()
{
	return "DX11";
}
#endif