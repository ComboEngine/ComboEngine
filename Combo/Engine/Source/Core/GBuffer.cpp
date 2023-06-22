#include "pch.h"
#include "GBuffer.h"
#include "Core.h"
#ifdef COMBO_DIRECTX11
#include "DirectX11/GBufferDX11.h"
#endif

void GBuffer::Create(GBuffer** Gbuffer)
{
#ifdef COMBO_DIRECTX11
	*Gbuffer = new GBufferDX11();
#endif

	GBuffer* buffer = *Gbuffer;
	Framebuffer::Create(&buffer->Position, Core::s_Window->GetWidth(), Core::s_Window->GetHeight(), FramebufferTarget::Color);
	Framebuffer::Create(&buffer->Diffuse, Core::s_Window->GetWidth(), Core::s_Window->GetHeight(), FramebufferTarget::Color);
	Framebuffer::Create(&buffer->Normal, Core::s_Window->GetWidth(), Core::s_Window->GetHeight(), FramebufferTarget::Color);
}
