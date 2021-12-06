cbuffer constantBuff : register(b0)
{
    float4x4 posMatrix;
};

struct VertexInputs
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VertexOutputs
{
    float4 retPos : SV_POSITION;
    float2 retTexCoord : TEXCOORD;
};

VertexOutputs merge(VertexInputs input, VertexOutputs output)
{
    output.retPos = mul(float4(input.pos, 1.0f), posMatrix);
    output.retTexCoord = input.texCoord;
    return output;
}

VertexOutputs main(VertexInputs verData)
{
    VertexOutputs returnData;
    returnData = merge(verData, returnData);
    return returnData;
}