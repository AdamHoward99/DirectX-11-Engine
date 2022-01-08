cbuffer lightingBuffer : register(b0)
{
	float3 ambientLightingColour;
	float ambientLightingStrength;
};

SamplerState samplerState : SAMPLER : register(s0);
Texture2D tex : TEXTURE : register(t0);

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 coords : TEXCOORD;
};

float4 main(PixelInput data) : SV_TARGET
{
    float3 pixelColour = tex.Sample(samplerState, data.coords).xyz;
	float3 ambientLighting = ambientLightingColour * ambientLightingStrength;
	float3 finalColour = pixelColour * ambientLighting;
    return float4(finalColour, 1.0f);
}