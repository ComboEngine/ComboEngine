#include "pch.h"
#include "Mesh.h"
#include "Pipeline.h"
#include "Core.h"
#include "Input.h"
#include "Camera.h"
#include "Texture.h"

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

void Mesh::Render(Scope<Material> Mat,glm::vec3 Position, glm::quat Orientation, glm::vec3 Scale)
{
	for (Submesh submesh : Submeshes) {
		Pipeline pipeline;
		pipeline.Count = submesh.IndexBuffer.Get()->Size;
		pipeline.ShaderDataBuffer = ShaderDataBuffer;
		pipeline.Indexed = true;
		pipeline.Shader = Mat.Get()->Shader;
		pipeline.VertexBuffer = submesh.VertexBuffer;
		pipeline.IndexBuffer = submesh.IndexBuffer;

		if (Mat.Get()->Diffuse.UseTexture) {
			pipeline.Texture = Mat.Get()->Diffuse.ColorTexture;
		}
		else {
			pipeline.Texture.Set(nullptr);
		}

		glm::mat4 wvp = glm::mat4(1.0f);
		wvp = Camera::CalculateProjectionMatrix();
		wvp = wvp * Camera::CalculateViewMatrix();
		wvp = glm::translate(wvp, Position);
		wvp = wvp * glm::mat4_cast(Orientation);
		wvp = glm::scale(wvp, Scale);

		MeshShaderData data;
		data.WVP = XMMatrixTranspose(ConvertToXMMATRIX(wvp));
		data.DiffuseUseTexture = false;
		data.Diffuse = XMFLOAT4(Mat.Get()->Diffuse.Color.x, Mat.Get()->Diffuse.Color.y, Mat.Get()->Diffuse.Color.z, Mat.Get()->Diffuse.Color.w);

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
