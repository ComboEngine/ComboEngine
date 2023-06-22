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
    float4 Position = PositionTexture.Sample(SamplerPosition, float2(input.TexCoord.x, -input.TexCoord.y));
   
    return Position;
}