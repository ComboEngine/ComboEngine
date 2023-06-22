#include "pch.h"
#include "IndexBuffer.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/IndexBufferDX11.h>
#endif

void IndexBuffer::Create(IndexBuffer** indexBuffer, std::vector<uint32_t> Indices)
{
	//Select window api
#ifdef COMBO_DIRECTX11
	*indexBuffer = new IndexBufferDX11();
#endif

	IndexBuffer* IndexBufferPtr = *indexBuffer;
	IndexBufferPtr->Size = Indices.size();

	//Call API Window
	IndexBufferPtr->Init(Indices);

	//LOG(std::string("Created Index Buffer (" + std::to_string(Indices.size()) + " size)"))
}
