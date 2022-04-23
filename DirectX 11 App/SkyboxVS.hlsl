cbuffer cbPerFrame : register(b0)
{
    float4x4 WVPMatrix;
};

struct VertexInputs
{
    float3 posL : POSITION;
};

struct VertexOutputs
{
    float4 retPosH : SV_POSITION;
    float3 retPosL : RET_POSITION;
};

VertexOutputs main(VertexInputs verData)
{
    VertexOutputs returnData;
    
    //Use local vertex position as cubemap lookup vector
    returnData.retPosL = verData.posL;
    
    //Set z = w so that z/w = 1 so the skybox is always on the far plane
    returnData.retPosH = mul(float4(returnData.retPosL, 1.0f), WVPMatrix);
    
    return returnData;
}
