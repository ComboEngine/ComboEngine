#include "pch.h"
#include "Mesh.h"
#include "Pipeline.h"
#include "Core.h"
#include "Input.h"
#include "Camera.h"
#include "Texture.h"
#include <Editor/Editor.h>

void Mesh::Create(Mesh** Obj, std::vector<Submesh*> Submeshes)
{
	*Obj = new Mesh();

	Mesh* ObjPtr = *Obj;
	ObjPtr->Submeshes = Submeshes;

	ShaderDataBuffer::Create(&ObjPtr->ShaderDataBuffer, sizeof(MeshShaderData));
}

/*XMMATRIX ConvertToXMMATRIX(const glm::mat4& matrix)
{
	const float* glmMatrixData = glm::value_ptr(matrix);
	XMFLOAT4X4 xnaMatrix;
	XMStoreFloat4x4(&xnaMatrix, XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(glmMatrixData)));
	return XMLoadFloat4x4(&xnaMatrix);
}*/

void Mesh::Render(Material* Mat,glm::vec3 Position, glm::quat Orientation, glm::vec3 Scale)
{
	for (Submesh* submesh : Submeshes) {
		Material* MatFinal = Mat;
		if (submesh->Material != nullptr) {
			MatFinal = std::any_cast<Material*>(submesh->Material->GetHandle());
		}

		Pipeline pipeline;
		pipeline.Count = submesh->IndexBuffer->Size;
		pipeline.ShaderDataBuffer = ShaderDataBuffer;
		pipeline.Indexed = true;
		pipeline.Shader = MatFinal->Shader;
		pipeline.VertexBuffer = submesh->VertexBuffer;
		pipeline.IndexBuffer = submesh->IndexBuffer;
		pipeline.Textures = {};

		if (MatFinal->Diffuse.UseTexture) {
			pipeline.Textures.push_back(std::any_cast<Texture*>(MatFinal->Diffuse.ColorTexture->GetHandle()));
		}

		glm::mat4 wvp = glm::mat4(1.0f);
		wvp = Camera::CalculateProjectionMatrix();
		wvp = wvp * Camera::CalculateViewMatrix();

		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::translate(Model, Position);
		Model = Model * glm::mat4_cast(Orientation);
		Model = glm::scale(Model, Scale);


		MeshShaderData data;
		data.Model = glm::transpose(Model);
		data.WVP = glm::transpose(wvp);
		data.RotationMatrix = glm::mat4_cast(Orientation);
		data.DiffuseUseTexture = glm::vec4(MatFinal->Diffuse.UseTexture, 0, 0, 0);
		data.Diffuse = glm::vec4(MatFinal->Diffuse.Color.x, MatFinal->Diffuse.Color.y, MatFinal->Diffuse.Color.z, MatFinal->Diffuse.Color.w);

		pipeline.VulkanPushConstant = &data;

		ShaderDataBuffer->Update(&data);
		Core::s_Context->Draw(pipeline);
	}
}

void Submesh::Init(std::vector<Vertex> Vertices, std::vector<uint32_t> Indices)
{
	VertexBuffer::Create(&this->VertexBuffer, Vertices);
	IndexBuffer::Create(&this->IndexBuffer, Indices);
}
