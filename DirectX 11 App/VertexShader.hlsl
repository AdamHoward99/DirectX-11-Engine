struct VertexInputs
{
    float2 pos : POSITION;
    float3 colour : COLOUR;
};

struct VertexOutputs
{
    float4 retPos : SV_POSITION;
    float3 retColour : COLOUR;
};

VertexOutputs merge(VertexInputs input, VertexOutputs output)
{
    output.retPos = float4(input.pos, 0.0f, 1.0f);
    output.retColour = input.colour;
    return output;
}

VertexOutputs main(VertexInputs verData)
{
    VertexOutputs returnData;
    returnData = merge(verData, returnData);
    return returnData;
}