#pragma once

#include <Engine/EngineHeaders.h>
#include <Math/Color.h>
#include <Graphics/GPUShader.h>


class GPUShader;
class Material {
public:
	Color32 albedo = Color32(1,1,1,1);
	sakura_ptr<GPUShader> shader;
	static sakura_ptr<Material> Create();
	sakura_string Compile() {
		return "struct VOut\n" 
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
                "float4 PShader(VOut data) : SV_TARGET\n" 
                "{\n" 
                "    return float4(data.texCoord.x,data.texCoord.y,1,1);\n" 
            "}";
	}
};