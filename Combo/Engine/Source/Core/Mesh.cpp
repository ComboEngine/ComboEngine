#include "pch.h"
#include "Mesh.h"
#include "Pipeline.h"
#include "Core.h"


void Mesh::Create(Scope<Mesh>& Obj, std::vector<Submesh> Submeshes)
{
	Scope<Mesh>::Create(Obj);

	Obj.Get()->Submeshes = Submeshes;

	ShaderDataBuffer::Create(Obj.Get()->ShaderDataBuffer, sizeof(MeshShaderData));
}

XMMATRIX ConvertToXMMATRIX(const glm::mat4& matrix)
{
	const float* glmMatrixData = glm::value_ptr(matrix);
	XMFLOAT4X4 xnaMatrix;
	XMStoreFloat4x4(&xnaMatrix, XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(glmMatrixData)));
	return XMLoadFloat4x4(&xnaMatrix);
}

glm::mat4 CalculateMatrix(glm::quat orientation, glm::vec3 position)
{
	auto rotation = orientation;
	auto r = glm::mat4_cast(rotation);
	auto t = glm::translate(glm::mat4(1.f), position);
	auto view = glm::affineInverse(t * r);
	return view;
}


void Mesh::Render(Scope<Shader> shader)
{
	for (Submesh submesh : Submeshes) {
		Pipeline pipeline;
		pipeline.Count = submesh.IndexBuffer.Get()->Size;
		pipeline.ShaderDataBuffer = ShaderDataBuffer;
		pipeline.Indexed = true;
		pipeline.Shader = shader;
		pipeline.VertexBuffer = submesh.VertexBuffer;
		pipeline.IndexBuffer = submesh.IndexBuffer;

		glm::mat4 wvp = glm::mat4(1.0f);
		wvp = glm::perspective(0.4f * 3.14f, (float)Core::s_Window.Get()->GetWidth() / Core::s_Window.Get()->GetHeight(), 0.1f, 1000.0f);
		wvp = wvp * CalculateMatrix(glm::quat(glm::vec3(0, 0, 0)), glm::vec3(0, 0, 3));
		wvp = glm::translate(wvp, glm::vec3(0, 0, 0));
		wvp = glm::rotate(wvp, 0.0f, glm::vec3(0, 1, 0));
		wvp = glm::scale(wvp, glm::vec3(1, 1, 1));


		MeshShaderData data;
		data.WVP = XMMatrixTranspose(ConvertToXMMATRIX(wvp));

		ShaderDataBuffer.Get()->Update(&data);
		Core::s_Context.Get()->Draw(pipeline);
	}
}

void Submesh::Init(std::vector<Vertex> Vertices, std::vector<uint32_t> Indices)
{
	VertexBuffer::Create(this->VertexBuffer, Vertices);
	IndexBuffer::Create(this->IndexBuffer, Indices);

	this->Vertices = Vertices;
	this->Indices = Indices;
}
