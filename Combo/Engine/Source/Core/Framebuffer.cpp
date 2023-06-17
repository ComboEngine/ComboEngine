#include "pch.h"
#include "Framebuffer.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/FramebufferDX11.h>
#endif

void Framebuffer::Create(Scope<Framebuffer>& Obj, int Width, int Height,FramebufferTarget Target)
{
	Obj = Scope<Framebuffer>();
	//Select window api
#ifdef COMBO_DIRECTX11
	Obj.Set(new FramebufferDX11());
#endif

	Obj.Get()->Target = Target;
	Obj.Get()->Width = Width;
	Obj.Get()->Height = Height;

	Obj.Get()->Init();
}
