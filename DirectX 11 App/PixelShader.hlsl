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
    return float4(pixelColour, 1.0f);
}