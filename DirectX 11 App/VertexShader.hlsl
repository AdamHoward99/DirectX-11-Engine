cbuffer constantBuff : register(b0)
{
    float4x4 WorldViewProjectionMatrix;
    float4x4 WorldMatrix;
};

struct VertexInputs
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
	float3 normCoord : NORMAL;
};

struct VertexOutputs
{
    float4 retPos : SV_POSITION;
    float2 retTexCoord : TEXCOORD;
	float3 retNormCoord : NORMAL;
    float3 retWorldPos : WORLD_POSITION;
};

VertexOutputs merge(VertexInputs input, VertexOutputs output)
{
	output.retPos = mul(float4(input.pos, 1.0f), WorldViewProjectionMatrix);
    output.retTexCoord = input.texCoord;
	output.retNormCoord = normalize(mul(float4(input.normCoord, 0.f), WorldMatrix));
	output.retWorldPos = mul(float4(input.pos, 1.f), WorldMatrix);
    return output;
}

VertexOutputs main(VertexInputs verData)
{
    VertexOutputs returnData;
    returnData = merge(verData, returnData);
    return returnData;
}