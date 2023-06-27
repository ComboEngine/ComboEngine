#include "pch.h"
#include "Framebuffer.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/FramebufferDX11.h>
#endif
#ifdef COMBO_VULKAN
#include <Core/Vulkan/FramebufferVulkan.h>
#endif

void Framebuffer::Create(Framebuffer** Obj, int Width, int Height,FramebufferTarget Target)
{
	//Select window api
#ifdef COMBO_DIRECTX11
	*Obj = new FramebufferDX11();
#endif
#ifdef COMBO_VULKAN
	*Obj = new FramebufferVulkan();
#endif
	Framebuffer* ObjPtr = *Obj;
	ObjPtr->Target = Target;
	ObjPtr->Width = Width;
	ObjPtr->Height = Height;

	ObjPtr->Init();
}
