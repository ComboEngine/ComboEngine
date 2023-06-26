#ifdef COMBO_VULKAN
#pragma once
#include <Core/ShaderDataBuffer.h>

class ShaderDataBufferVulkan : public ShaderDataBuffer {
public:
	size_t Size;
	virtual void Init(size_t Size);
	virtual void Update(const void* Data);
	virtual std::string GetApiName();
};
#endif