struct Light
{
    float4 LightTypeAndPos;
    float4 LightRadiusAndColor;
};

cbuffer LightingBufferPass
{
    float4 LightCountAndCameraPos;
    Light lights[1000];
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

float4 PSMain(PSInput input) : SV_Target
{
    int3 sampleIndices = int3(input.Position.xy, 0);
    float3 Normal = NormalTexture.Load(sampleIndices).xyz;
    float3 Position = PositionTexture.Load(sampleIndices).xyz;
    float3 Diffuse = DiffuseTexture.Load(sampleIndices).xyz;
    
    float3 lighting = Diffuse * 0.1;
    float3 viewDir = normalize(LightCountAndCameraPos.yzw - Position);
    
    for (int i = 0; i < LightCountAndCameraPos.x; i++)
    {
        Light light = lights[i];
        //0 = DirectionalLight
        //1 = PointLight
        if (light.LightTypeAndPos.x == 0)
        {
            float3 lightDir = normalize(light.LightTypeAndPos.yzw);
            float3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.LightRadiusAndColor.yzw;
            float3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            float3 specular = light.LightRadiusAndColor.yzw * spec * 0;
            lighting += diffuse + specular;
        }
        else if (light.LightTypeAndPos.x == 1)
        {
            float distance = length(light.LightTypeAndPos.yzw - Position);
            float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));
            
            float3 lightDir = normalize(light.LightTypeAndPos.yzw - Position);
            float3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.LightRadiusAndColor.yzw;
            
            float3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            float3 specular = light.LightRadiusAndColor.yzw * spec * 0;
            
            diffuse *= attenuation;
            specular *= attenuation;
            
            lighting += diffuse + specular;
        }
    }
    
    
    float alpha = 1;
    if (length(Position) <= 0)
    {
        alpha = 0;
    }
   
    return float4(lighting, alpha);
}