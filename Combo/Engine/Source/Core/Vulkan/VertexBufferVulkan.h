#ifdef COMBO_VULKAN
#pragma once
#include <Core/VertexBuffer.h>

class VertexBufferVulkan : public VertexBuffer {
public:
	VkBuffer Buffer;
	VkDeviceMemory Memory;
	uint32_t Size;
	virtual void Init(std::vector<Vertex> Vertices);
	virtual std::string GetApiName();
};
#endif