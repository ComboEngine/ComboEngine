struct PSInput
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

struct VSInput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    
    output.Position = input.Position;
    output.Color = input.Color;
    
    return output;
}

float4 PSMain(PSInput input) : SV_Target
{
    return input.Color;
}