#include "pch.h"
#include "PostFX.h"
#include "Core.h"
#include "GlobalShaders.h"
#include "Camera.h"

void PostFXRenderer::Init()
{
	std::vector<Vertex> vertices = {
		{1.0f,1.0f,0.0f,	1.0f,1.0f,	0.0f,0.0f,0.0f,0.0f},
		{1.0f,-1.0f,0.0f,	1.0f,0.0f,	0.0f,0.0f,0.0f,0.0f},
		{-1.0f,-1.0f,0.0f,	0.0f,0.0f,	0.0f,0.0f,0.0f,0.0f},
		{-1.0f,1.0f,0.0f,	0.0f,1.0f,	0.0f,0.0f,0.0f,0.0f}
	};

	std::vector<uint32_t> indices = {
		0,1,3,1,2,3
	};

	VertexBuffer::Create(&QuadVertexBuffer, vertices);
	IndexBuffer::Create(&QuadIndexBuffer, indices);
	ShaderDataBuffer::Create(&LightingBuffer, sizeof(LightingBufferPass));

	Framebuffer::Create(&Frame, Core::s_Window->GetWidth(), Core::s_Window->GetHeight(), FramebufferTarget::Color);
}

void PostFXRenderer::Draw()
{
	Frame->Bind(false);
	Core::s_Context->BeginDraw();
	Pipeline pipeline;
	pipeline.Shader = GlobalShaders::GetShader(GlobalShader::PostFX);
	pipeline.Count = 6;
	pipeline.IndexBuffer = QuadIndexBuffer;
	pipeline.VertexBuffer = QuadVertexBuffer;
	pipeline.Indexed = true;
	pipeline.ShaderDataBuffer = LightingBuffer;
	pipeline.Textures = Core::s_GBuffer->GetTextureArray();

	LightingBufferPass lighting;
	lighting.CameraPos = XMFLOAT3(Camera::Position.x, Camera::Position.y, Camera::Position.z);

	LightingBuffer->Update(&lighting);
	Core::s_Context->Draw(pipeline);
	Frame->Unbind();
}
