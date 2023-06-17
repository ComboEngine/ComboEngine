cbuffer ConstantBuffer
{
    float4x4 WVP;
    float4 Diffuse;
    int DiffuseUseTexture;
};

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
    
    output.Position = mul(input.Position,WVP);
    //output.Position = input.Position;
    output.TexCoord = input.TexCoord;
    output.Normal = input.Normal;
    
    return output;
}

Texture2D Texture;
SamplerState Sampler;
float4 PSMain(PSInput input) : SV_Target
{
    float4 MaterialDiffuse = float4(1, 1, 1, 1);
    if (DiffuseUseTexture)
    {
        MaterialDiffuse = Texture.Sample(Sampler, input.TexCoord);
    }
    else
    {
        MaterialDiffuse = Diffuse;
    }
    
    return MaterialDiffuse;
}