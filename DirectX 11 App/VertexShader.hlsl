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
    float3 tangetCoord : TANGENT;
};

struct VertexOutputs
{
    float4 retPos : SV_POSITION;
    float2 retTexCoord : TEXCOORD;
    float3 retNormCoord : NORMAL;
    float3 retTangentCoord : TANGENT;
    float3 retWorldPos : WORLD_POSITION;
};

VertexOutputs merge(VertexInputs input, VertexOutputs output)
{
	output.retPos = mul(float4(input.pos, 1.0f), WorldViewProjectionMatrix);
    output.retTexCoord = input.texCoord;
    output.retNormCoord = mul(float4(input.normCoord, 0.f), WorldMatrix).xyz;
    output.retTangentCoord = mul(input.tangetCoord, (float3x3) WorldMatrix);
    output.retWorldPos = mul(float4(input.pos, 1.f), WorldMatrix).xyz;
    return output;
}

VertexOutputs main(VertexInputs verData)
{
    VertexOutputs returnData;
    returnData = merge(verData, returnData);
    return returnData;
}