#include "pch.h"
#include "Material.h"
#include <Engine/Engine.h>

void Material::Init()
{
    this->shader = GPUShader::Create(this->Compile());
}

std::shared_ptr<Material> Material::Create()
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	Engine::Materials.push_back(material);

	return material;
}

std::string Material::Compile()
{
    std::ostringstream oss;
    oss << 
        "cbuffer ConstantBuffer\n"
        "{\n"
        "    float4x4 WVP;\n"
        "};\n"
        "struct VOut\n"
        "{\n"
        "    float4 position : SV_POSITION;\n"
        "    float2 texCoord : TEXCOORD;\n"
        "	 float4 normal : NORMAL; \n"
        "};\n"
        "\n"
        "VOut VShader(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL)\n"
        "{\n"
        "    VOut output;\n"
        "\n"
        "    output.position = mul(position,WVP);\n"
        "    output.texCoord = float2(texCoord.x,-texCoord.y);\n"
        "    output.normal = normal;\n"
        "\n"
        "    return output;\n"
        "}\n"
        "\n"
        "\n"
        "\n"
        "Texture2D Texture;\n"
        "SamplerState TextureSampler;\n"
        "float4 PShader(VOut data) : SV_TARGET {\n";


    //oss << "    return float4(" << albedo.R << ", " << albedo.G << ", " << albedo.B << ", " << albedo.A << ");\n";
   // oss << "    return float4(data.texCoord.x,data.texCoord.y,1,1);\n";
    oss << "    return Texture.Sample(TextureSampler, data.texCoord);\n";

    oss << "}";
    return oss.str();
}