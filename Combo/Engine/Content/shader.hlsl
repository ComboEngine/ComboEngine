cbuffer ConstantBuffer
{
    int Stage;
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
    output.TexCoord = input.TexCoord;
    output.Normal = input.Normal;
    
    return output;
}

Texture2D Texture;
SamplerState Sampler;
float4 PSMain(PSInput input) : SV_Target
{
    input.Normal = normalize(input.Normal);
    float4 MaterialDiffuse = float4(1, 1, 1, 1);
    if (DiffuseUseTexture)
    {
        MaterialDiffuse = Texture.Sample(Sampler, input.TexCoord);
    }
    else
    {
        MaterialDiffuse = Diffuse;
    }
    
    MaterialDiffuse = float4(input.TexCoord, 1, 1);
    
    float3 dir = float3(0.25f, 0.5f, -1.0f);
    float4 ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
    float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float3 finalColor;

    finalColor = MaterialDiffuse * ambient;
    finalColor += saturate(dot(dir, input.Normal) * diffuse * MaterialDiffuse);
    
    return float4(finalColor, MaterialDiffuse.a);
}