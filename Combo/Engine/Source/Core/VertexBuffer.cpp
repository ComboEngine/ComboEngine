#include "pch.h"
#include "VertexBuffer.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/VertexBufferDX11.h>
#endif
#ifdef COMBO_VULKAN
#include <Core/Vulkan/VertexBufferVulkan.h>
#endif

void VertexBuffer::Create(VertexBuffer** vertexBuffer, std::vector<Vertex> Vertices)
{
	//Select window api
#ifdef COMBO_DIRECTX11
	*vertexBuffer = new VertexBufferDX11();
#endif
#ifdef COMBO_VULKAN
	*vertexBuffer = new VertexBufferVulkan();
#endif


	VertexBuffer* vbPtr = *vertexBuffer;
	vbPtr->Size = Vertices.size();

	//Call API Window
	vbPtr->Init(Vertices);

	//LOG(std::string("Created Vertex Buffer (" + std::to_string(Vertices.size()) + " size)"))
}
