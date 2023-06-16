#include "pch.h"
#include "VertexBuffer.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/VertexBufferDX11.h>
#endif

void VertexBuffer::Create(Scope<VertexBuffer>& vertexBuffer, std::vector<Vertex> Vertices)
{
	vertexBuffer = Scope<VertexBuffer>();
	//Select window api

#ifdef COMBO_DIRECTX11
	vertexBuffer.Set(new VertexBufferDX11());
#endif

	vertexBuffer.Get()->Size = Vertices.size();

	//Call API Window
	vertexBuffer.Get()->Init(Vertices);

	LOG(std::string("Created Vertex Buffer (" + std::to_string(Vertices.size()) + " size)"))
}
