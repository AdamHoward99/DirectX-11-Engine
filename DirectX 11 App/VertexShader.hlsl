cbuffer constantBuff : register(b0)
{
    float xOff;
    float yOff;
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
    output.retPos = float4(input.pos, 1.0f);
    output.retTexCoord = input.texCoord;
    return output;
}

VertexOutputs main(VertexInputs verData)
{
    VertexOutputs returnData;
    verData.pos.x += xOff;
    verData.pos.y += yOff;
    returnData = merge(verData, returnData);
    return returnData;
}