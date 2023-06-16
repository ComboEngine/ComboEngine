cbuffer ConstantBuffer
{
    float4x4 WVP;
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

float4 PSMain(PSInput input) : SV_Target
{
    return float4(input.TexCoord, 1, 1);
}