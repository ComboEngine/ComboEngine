#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Framebuffer.h"
#include "ShaderDataBuffer.h"
struct LightBufferPass
{
	glm::vec4 LightTypeAndPos;
	glm::vec4 LightRadiusAndColor;
};

struct LightingBufferPass
{
	glm::vec4 LightCountAndCameraPos;
	LightBufferPass lights[1000];
};

class PostFXRenderer {
public:
	Framebuffer* Frame;
	VertexBuffer* QuadVertexBuffer;
	IndexBuffer* QuadIndexBuffer;
	ShaderDataBuffer* LightingBuffer;
	void Init();
	void Draw();
};