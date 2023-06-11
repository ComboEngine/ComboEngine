cbuffer ConstantBuffer
{
    float4x4 WVP;
};
struct VOut
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
	float4 normal : NORMAL; 
};

VOut VShader(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL)
{
    VOut output;

    output.position = mul(position,WVP);
    output.texCoord = float2(texCoord.x,-texCoord.y);
    output.normal = normal;

    return output;
}


Texture2D Texture;
SamplerState TextureSampler;
float4 PShader(VOut data) : SV_TARGET {;
     return float4(1,1,1,1);
};