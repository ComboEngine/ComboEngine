#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Framebuffer.h"
#include "ShaderDataBuffer.h"

struct LightingBufferPass {
	XMFLOAT3 CameraPos;
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