#include "pch.h"
#include "Texture.h"
#ifdef COMBO_DIRECTX11
#include "DirectX11/TextureDX11.h"
#endif

void Texture::Create(Scope<Texture>& Obj, void* mipData, int Width, int Height)
{
	Obj = Scope<Texture>();

#ifdef COMBO_DIRECTX11
	Obj.Set(new TextureDX11());
#endif
	//Call
	Obj.Get()->Init(mipData,Width,Height);

	LOG(std::string("Created Texture (") + std::to_string(sizeof(mipData)) + std::string(")"));
}
