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
	Scope<Shader> Shader;
	Scope<VertexBuffer> VertexBuffer;
	Scope<IndexBuffer> IndexBuffer;
	Scope<ShaderDataBuffer> ShaderDataBuffer;
	std::vector<Scope<Texture>> Textures;
};