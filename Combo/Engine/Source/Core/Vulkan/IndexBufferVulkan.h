#ifdef COMBO_VULKAN
#pragma once
#include <Core/IndexBuffer.h>

class IndexBufferVulkan : public IndexBuffer {
public:
	VkBuffer Buffer;
	VkDeviceMemory Memory;
	uint32_t Size;
	virtual void Init(std::vector<uint32_t> Indices);
	virtual std::string GetApiName();
};
#endif