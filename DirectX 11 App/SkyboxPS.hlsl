TextureCube cubeMap : register(t0);
SamplerState LinearWrapSS : SAMPLER : register(s0);

struct PixelInput
{
    float4 posH : SV_POSITION;
    float3 posL : POSITION;
};

float4 main(PixelInput pin) : SV_TARGET
{
    return cubeMap.Sample(LinearWrapSS, pin.posL);
}