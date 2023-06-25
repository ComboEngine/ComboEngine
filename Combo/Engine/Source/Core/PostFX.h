#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Framebuffer.h"
#include "ShaderDataBuffer.h"
struct LightBufferPass
{
	XMFLOAT4 LightTypeAndPos;
	XMFLOAT4 LightRadiusAndColor;
};

struct LightingBufferPass
{
	XMFLOAT4 LightCountAndCameraPos;
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