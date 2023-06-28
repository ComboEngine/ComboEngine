#include "pch.h"
#include "Texture.h"
#ifdef COMBO_DIRECTX11
#include "DirectX11/TextureDX11.h"
#endif
#ifdef COMBO_VULKAN
#include "Vulkan/TextureVulkan.h"
#endif

void Texture::Create(Texture** Obj, void* mipData, int Width, int Height)
{
#ifdef COMBO_DIRECTX11
	*Obj = new TextureDX11();
#endif
#ifdef COMBO_VULKAN
	*Obj = new TextureVulkan();
#endif
	//Call
	Texture* ObjPtr = *Obj;
	ObjPtr->Init(mipData,Width,Height);

	LOG(std::string("Created Texture (") + std::to_string(sizeof(mipData)) + std::string(")"));
}
