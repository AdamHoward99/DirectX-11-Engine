struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 colour : COLOUR;
};

float4 main(PixelInput data) : SV_TARGET
{
    return float4(data.colour, 1.0f);
}