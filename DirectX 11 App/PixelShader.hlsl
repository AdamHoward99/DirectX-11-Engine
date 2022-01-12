cbuffer lightingBuffer : register(b0)
{
	float3 ambientLightingColour;
	float ambientLightingStrength;

	float3 dynamicLightingColour;
	float dynamicLightingStrength;
	float3 dynamicLightingPosition;
};

SamplerState samplerState : SAMPLER : register(s0);
Texture2D tex : TEXTURE : register(t0);

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 coords : TEXCOORD;
	float3 normals : NORMAL;
};

float4 main(PixelInput data) : SV_TARGET
{
    float3 pixelColour = tex.Sample(samplerState, data.coords).xyz;
	float3 normalColour = data.normals;
	
	float3 finalColour = pixelColour;
    return float4(finalColour, 1.0f);
}