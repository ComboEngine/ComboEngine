cbuffer ConstantBuffer
{
    float4x4 WVP;
    float4x4 Model;
    float4 Diffuse;
    int DiffuseUseTexture;
};

struct PSInput
{
    float4 Position : SV_Position;
    float3 PositionWithModel : POSITION;
    float2 TexCoord : TEXCOORD;
    float4 Normal : NORMAL;
};

struct VSInput
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD;
    float4 Normal : NORMAL;
};

struct PSOutput
{
    float4 Position : SV_Target0;
    float4 Diffuse : SV_Target1;
    float4 Normal : SV_Target2;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    
    output.Position = mul(input.Position, WVP);
    output.PositionWithModel = float3(mul(Model, input.Position).xyz);
    output.TexCoord = input.TexCoord;
    output.Normal = input.Normal;
    
    return output;
}

float4 CalculateMaterialData(PSInput input, Texture2D Texture, SamplerState Sampler, int check, float4 Diffuse)
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

Texture2D Texture;
SamplerState Sampler;
PSOutput PSMain(PSInput input) : SV_Target
{
    PSOutput output;
   
    input.Normal = normalize(input.Normal);
    float4 MaterialDiffuse = CalculateMaterialData(input, Texture, Sampler, DiffuseUseTexture, Diffuse);
    
   // MaterialDiffuse = float4(input.TexCoord, 1, 1);
        
    output.Position = float4(input.PositionWithModel,1);
    output.Diffuse = MaterialDiffuse;
    output.Normal = input.Normal;
   
    return output;
}