cbuffer lightingBuffer : register(b0)
{
	float3 ambientLightingColour;
	float ambientLightingStrength;
};

struct PointLightData
{
    float3 dynamicLightingColour;
    float dynamicLightingStrength;
    float3 dynamicLightingPosition;
};

cbuffer PointLightBuffer : register(b1) 
{
    PointLightData pLights[2];
};

SamplerState samplerState : SAMPLER : register(s0);
Texture2D tex : TEXTURE : register(t0);

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 coords : TEXCOORD;
	float3 normals : NORMAL;
    float4 worldPos : WORLD_POSITION;
};

float4 main(PixelInput data) : SV_TARGET
{
	///Obtain colour of the texture
    float3 pixelColour = tex.Sample(samplerState, data.coords).xyz;
	///Obtain normal of the texture
	float3 normalColour = data.normals;
	///Obtain the ambient lighting
	float3 ambientLight = ambientLightingColour * ambientLightingStrength;
    
    float3 combinedLight;
    for (int i = 0; i < 2; i++)
    {
	    ///Obtain vector from pixel to light
        float3 vectorToLightSource = normalize(pLights[i].dynamicLightingPosition - data.worldPos.xyz);
	    ///Get Diffuse light intensity
	    float3 diffuseLightInt = max(dot(vectorToLightSource, data.normals), 0.f);
	    ///Calculate Diffuse lighting
        float3 diffuseLight = diffuseLightInt * pLights[i].dynamicLightingStrength * pLights[i].dynamicLightingColour;
        combinedLight += diffuseLight;
    }
    
 //   	///Obtain vector from pixel to light
 //   float3 vectorToLight2Source = normalize(pLight2.dynamicLightingPosition - data.worldPos.xyz);
	/////Get Diffuse light intensity
 //   float3 diffuseLight2Int = max(dot(vectorToLight2Source, data.normals), 0.f);
	/////Calculate Diffuse lighting
 //   float3 diffuseLight2 = diffuseLight2Int * pLight2.dynamicLightingStrength * pLight2.dynamicLightingColour;
    
	///Obtain overall final lighting for pixel
	float3 finalLight = ambientLight + combinedLight;
	///Obtain overall final colour of pixel
	float3 finalColour = pixelColour * saturate(finalLight);
    return float4(finalColour, 1.0f);
}