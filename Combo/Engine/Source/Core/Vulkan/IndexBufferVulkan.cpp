#include "pch.h"
#ifdef COMBO_VULKAN
#include <Core/IndexBuffer.h>
#include "IndexBufferVulkan.h"
#include <Core/Core.h>
#include "ContextVulkan.h"

void IndexBufferVulkan::Init(std::vector<uint32_t> Indices)
{
	Size = static_cast<uint32_t>(Indices.size());
	ContextVulkan* context = ContextVulkan::GetVulkanContext();
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(Indices[0]) * Indices.size();
	bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CB_CHECKHR(vkCreateBuffer(context->Device, &bufferInfo, nullptr, &Buffer));

	VkMemoryRequirements req;
	vkGetBufferMemoryRequirements(context->Device, Buffer, &req);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = req.size;
	allocInfo.memoryTypeIndex = context->FindMemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	CB_CHECKHR(vkAllocateMemory(context->Device, &allocInfo, nullptr, &Memory));

	vkBindBufferMemory(context->Device, Buffer, Memory, 0);

	void* data;
	vkMapMemory(context->Device, Memory, 0, bufferInfo.size, 0, &data);
	memcpy(data, Indices.data(), (size_t)bufferInfo.size);
	vkUnmapMemory(context->Device, Memory);
}

std::string IndexBufferVulkan::GetApiName()
{
	return "Vulkan";
}


#endif