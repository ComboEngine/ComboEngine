#include "pch.h"
#ifdef COMBO_DIRECTX11
#include <Core/ShaderDataBuffer.h>
#include "ShaderDataBufferDX11.h"
#include <Core/Core.h>
#include "ContextDX11.h"

void ShaderDataBufferDX11::Init(size_t Size)
{
	ContextDX11* context = reinterpret_cast<ContextDX11*>(Core::s_Context);
	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth = Size*16;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = 0;

	CB_CHECKHR(context->Device->CreateBuffer(&BufferDesc, NULL, &this->Buffer));

	Core::ExitEvent.Hook([&] {
		this->Buffer->Release();
	});
}

void ShaderDataBufferDX11::Update(const void* Data)
{
	reinterpret_cast<ContextDX11*>(Core::s_Context)->Context->UpdateSubresource(this->Buffer, 0, NULL, Data, 0, 0);
}

std::string ShaderDataBufferDX11::GetApiName()
{
	return "DX11";
}


#endif