#include "Material.h"
#include <Engine/Engine.h>

void Material::Init()
{
    this->shader = GPUShader::Create(this->Compile());
}

sakura_ptr<Material> Material::Create()
{
	sakura_ptr<Material> material = make_shared<Material>();
	Engine::Materials.push_back(material);

	return material;
}

sakura_string Material::Compile()
{
    std::ostringstream oss;
    oss << "struct VOut\n"
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
        "    output.position = position;\n"
        "    output.texCoord = texCoord;\n"
        "    output.normal = normal;\n"
        "\n"
        "    return output;\n"
        "}\n"
        "\n"
        "\n"
        "\n"
        "float4 PShader(VOut data) : SV_TARGET {\n";


    oss << "    return float4(" << albedo.R << ", " << albedo.G << ", " << albedo.B << ", " << albedo.A << ");\n";

    oss << "}";
    return oss.str();
}