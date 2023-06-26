#pragma once
#ifdef COMBO_VULKAN
#include "pch.h"
#endif

struct Vertex { 
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec4 Normal;
	Vertex(float X, float Y, float Z, float texCoordX, float texCoordY, float normalX, float normalY, float normalZ, float normalW) : Position(X,Y,Z),TexCoord(texCoordX,texCoordY), Normal(normalX,normalY,normalZ,normalW) {
	}
	Vertex() {}
#ifdef COMBO_VULKAN
	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, Position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, TexCoord);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, Normal);

		return attributeDescriptions;
	}
#endif
};