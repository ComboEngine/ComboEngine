#pragma once
#include "pch.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderDataBuffer.h"
#include "Texture.h"

struct Pipeline {
	int Count;
	bool Indexed;
	Shader* Shader;
	VertexBuffer* VertexBuffer;
	IndexBuffer* IndexBuffer;
	ShaderDataBuffer* ShaderDataBuffer;
	std::vector<Texture*> Textures;
	const void* VulkanPushConstant;
};