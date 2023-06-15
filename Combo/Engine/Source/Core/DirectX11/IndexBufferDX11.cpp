#include "pch.h"
#ifdef COMBO_DIRECTX11
#include <Core/IndexBuffer.h>
#include "IndexBufferDX11.h"
#include <Core/Core.h>
#include "ContextDX11.h"

void IndexBufferDX11::Init(std::vector<int> Indices)
{
	ContextDX11* context = Core::s_Context.Cast<ContextDX11>();
	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth = sizeof(int) * Indices.size();
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA BufferInitData;
	BufferInitData.pSysMem = Indices.data();
	CB_CHECKHR(context->Device->CreateBuffer(&BufferDesc, &BufferInitData, &this->Buffer))
}

std::string IndexBufferDX11::GetApiName()
{
	return "DX11";
}


#endif