#include "pch.h"
#ifdef COMBO_DIRECTX11
#include <Core/VertexBuffer.h>
#include "VertexBufferDX11.h"
#include <Core/Core.h>
#include "ContextDX11.h"

void VertexBufferDX11::Init(std::vector<Vertex> Vertices)
{
	ContextDX11* context = Core::s_Context.Cast<ContextDX11>();
	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(Vertex) * Vertices.size();
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA BufferInitData{};
	BufferInitData.pSysMem = Vertices.data();
	CB_CHECKHR(context->Device->CreateBuffer(&BufferDesc, &BufferInitData, &this->Buffer));

	Core::ExitEvent.Hook([&] {
		this->Buffer->Release();
	});
}

std::string VertexBufferDX11::GetApiName()
{
	return "DX11";
}


#endif