#ifdef COMBO_VULKAN
#pragma once
#include <Core/Shader.h>

class ShaderVulkan : public Shader {
public:
	VkShaderModule Vertex;
	VkShaderModule Fragment;
	VkPipeline GraphicsPipeline;
	VkPipelineLayout PipelineLayout;
	virtual void Init(std::string VertexSource, std::string PixelSource);
	virtual std::string GetApiName();
};
#endif