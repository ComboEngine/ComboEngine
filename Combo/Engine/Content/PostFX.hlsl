struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
    float4 Normal : NORMAL;
};

struct VSInput
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD;
    float4 Normal : NORMAL;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    
    output.Position = input.Position;
    output.TexCoord = input.TexCoord;
    output.Normal = input.Normal;
    
    return output;
}

Texture2D PositionTexture : register(t0);
SamplerState SamplerPosition : register(s0);

Texture2D DiffuseTexture : register(t1);
SamplerState SamplerDiffuse : register(s1);

Texture2D NormalTexture : register(t2);
SamplerState SamplerNormal : register(s2);

SamplerState linear_sample
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
    MaxAnisotropy = 16;
    MaxLOD = 3.402823466e+38f;
};
float4 PSMain(PSInput input) : SV_Target
{
    int3 sampleIndices = int3(input.Position.xy, 0);
    float3 Normal = NormalTexture.Load(sampleIndices).xyz;
    float3 Position = PositionTexture.Load(sampleIndices).xyz;
    float3 Diffuse = DiffuseTexture.Load(sampleIndices).xyz;
    
    float3 L = -float3(10.0f, -10.0f, 0.0f);

    float lightAmountDL = saturate(dot(Normal, L));
    float3 color = float3(1, 1, 1) * lightAmountDL * Diffuse;
   
    return float4(color, 1.0f);
}