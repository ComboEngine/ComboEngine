#include "pch.h"
#ifdef COMBO_VULKAN
#include "ShaderVulkan.h"
#include "ContextVulkan.h"
#include <Core/Core.h>
#include "../Mesh.h"

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
void ShaderVulkan::Init(std::string VertexSource, std::string PixelSource)
{
    ContextVulkan* context = reinterpret_cast<ContextVulkan*>(Core::s_Context);
    auto vertShaderCode = readFile(VertexSource);
    auto fragShaderCode = readFile(PixelSource);

    VkShaderModuleCreateInfo createInfoVertex{};
    createInfoVertex.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfoVertex.codeSize = vertShaderCode.size();
    createInfoVertex.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());

    VkShaderModuleCreateInfo createInfoFragment{};
    createInfoFragment.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfoFragment.codeSize = fragShaderCode.size();
    createInfoFragment.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());

    CB_CHECKHR(vkCreateShaderModule(context->Device, &createInfoVertex, nullptr, &this->Vertex));
    CB_CHECKHR(vkCreateShaderModule(context->Device, &createInfoFragment, nullptr, &this->Fragment));

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = Vertex;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = Fragment;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)context->GetSwapChain()->SwapChainExtent.width;
    viewport.height = (float)context->GetSwapChain()->SwapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = context->GetSwapChain()->SwapChainExtent;

    std::vector<VkDynamicState> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendPositionAttachment{};
    colorBlendPositionAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendPositionAttachment.blendEnable = VK_FALSE;
    colorBlendPositionAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendPositionAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendPositionAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendPositionAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendPositionAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendPositionAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendAttachmentState colorBlendDiffuseAttachment{};
    colorBlendDiffuseAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendDiffuseAttachment.blendEnable = VK_FALSE;
    colorBlendDiffuseAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendDiffuseAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendDiffuseAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendDiffuseAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendDiffuseAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendDiffuseAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendAttachmentState colorBlendNormalAttachment{};
    colorBlendNormalAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendNormalAttachment.blendEnable = VK_FALSE;
    colorBlendNormalAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendNormalAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendNormalAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendNormalAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendNormalAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendNormalAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    std::array<VkPipelineColorBlendAttachmentState, 3> blendAttachments;
    blendAttachments[0] = colorBlendPositionAttachment;
    blendAttachments[1] = colorBlendDiffuseAttachment;
    blendAttachments[2] = colorBlendNormalAttachment;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = blendAttachments.size();
    colorBlending.pAttachments = blendAttachments.data();
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional

    std::array<VkFormat, 3> Formats;
    Formats[0] = VK_FORMAT_R16G16B16A16_SFLOAT;
    Formats[1] = VK_FORMAT_R16G16B16A16_SFLOAT;
    Formats[2] = VK_FORMAT_R8G8B8A8_UNORM;

    VkPipelineRenderingCreateInfoKHR rendering{};
    rendering.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    rendering.colorAttachmentCount = Formats.size();
    rendering.pColorAttachmentFormats = Formats.data();
    rendering.depthAttachmentFormat = context->GetSwapChain()->DepthFormat;

    VkPushConstantRange push_constant;
    push_constant.offset = 0;
    push_constant.size = sizeof(MeshShaderData);
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 1; // Optional
    pipelineLayoutInfo.pPushConstantRanges = &push_constant; // Optional

    CB_CHECKHR(vkCreatePipelineLayout(context->Device, &pipelineLayoutInfo, nullptr, &PipelineLayout));

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.pNext = &rendering;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = PipelineLayout;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    CB_CHECKHR(vkCreateGraphicsPipelines(context->Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &GraphicsPipeline));
}
std::string ShaderVulkan::GetApiName()
{
	return "Vulkan";
}
#endif