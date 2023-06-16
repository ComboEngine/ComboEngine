#include "pch.h"
#include "IndexBuffer.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/IndexBufferDX11.h>
#endif

void IndexBuffer::Create(Scope<IndexBuffer>& indexBuffer, std::vector<uint32_t> Indices)
{
	indexBuffer = Scope<IndexBuffer>();
	//Select window api

#ifdef COMBO_DIRECTX11
	indexBuffer.Set(new IndexBufferDX11());
#endif

	indexBuffer.Get()->Size = Indices.size();

	//Call API Window
	indexBuffer.Get()->Init(Indices);

	LOG(std::string("Created Index Buffer (" + std::to_string(Indices.size()) + " size)"))
}
