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

	LightingBufferPass lighting{};
	lighting.LightCountAndCameraPos = glm::vec4(Core::Scene.LightingData.size(),Camera::Position.x, Camera::Position.y, Camera::Position.z);


	for (int i = 0; i < Core::Scene.LightingData.size(); i++) {
		Light light = Core::Scene.LightingData[i];
		LightBufferPass pass{};
		pass.LightTypeAndPos = glm::vec4(light.Type, light.Direction.x, light.Direction.y, light.Direction.z);
		pass.LightRadiusAndColor = glm::vec4(0, light.Color.x, light.Color.y, light.Color.z);

		if (light.Type == Point) {
			float constant = 1.0;
			float linear = 0.7;
			float quadratic = 1.8;
			float lightMax = std::fmaxf(std::fmaxf(light.Color.x, light.Color.y), light.Color.z);
			float radius =
				(-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
				/ (2 * quadratic);
			pass.LightRadiusAndColor.x = radius;
		}
		lighting.lights[i] = pass;
	}

	LightingBuffer->Update(&lighting);
	Core::s_Context->Draw(pipeline);
	Frame->Unbind();
}
