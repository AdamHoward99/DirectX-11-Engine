#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 1

cbuffer AmbientLightBuffer : register(b0)
{
	float3 ambientLightingColour;
	float ambientLightingStrength;
};

struct PointLightData
{
    float3 dynamicLightingColour;
    float dynamicLightingStrength;
    float3 dynamicLightingPosition;
    float lightFalloffStart;
    float lightFalloffEnd;
};

cbuffer PointLightBuffer : register(b1) 
{
    PointLightData pLights[NUM_POINT_LIGHTS];
};

struct SpotLightData
{
    float3 dynamicLightingColour; //12-bit
    float dynamicLightingStrength; //4-bit
    float3 dynamicLightPosition; //12-bit
    float3 dynamicLightRotation; //12-bit
};

cbuffer SpotLightBuffer : register(b2)
{
    SpotLightData SpLights[NUM_SPOT_LIGHTS];
}

SamplerState samplerState : SAMPLER : register(s0);
Texture2D tex : TEXTURE : register(t0);

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 coords : TEXCOORD;
	float3 normals : NORMAL;
    float4 worldPos : WORLD_POSITION;
};

float3 ComputePointLighting(PointLightData pLight, PixelInput data)
{
    //Obtain vector from pixel to light
    float3 vectorToLightSource = pLight.dynamicLightingPosition - data.worldPos.xyz;
    //Get Distance from pixel to light
    float distance = length(vectorToLightSource);
    //Check if light is out of range from pixel
    if (distance > pLight.lightFalloffEnd || abs(distance) < pLight.lightFalloffStart)
        return float3(0.f, 0.f, 0.f);
    //Normalize distance vector
    vectorToLightSource /= distance;
	//Get Diffuse light intensity
    float3 diffuseLightInt = max(dot(vectorToLightSource, data.normals), 0.f) * pLight.dynamicLightingStrength;
	///Calculate Diffuse lighting
    float3 diffuseLight = diffuseLightInt  * pLight.dynamicLightingColour;
    return diffuseLight;
}

float4 main(PixelInput data) : SV_TARGET
{
	///Obtain colour of the texture
    float3 pixelColour = tex.Sample(samplerState, data.coords).xyz;
	///Obtain normal of the texture
	float3 normalColour = data.normals;
	///Obtain the ambient lighting
	float3 ambientLight = ambientLightingColour * ambientLightingStrength;
    
    ///Stores combined equation of all point lights in scene
    float3 overallPointLighting;
    for (int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        overallPointLighting += ComputePointLighting(pLights[i], data);
        
        //float spotFactor = pow(max(dot(-vectorToLightSource, float3(0.f, -1.f, 0.f)), 0.f), 1.f);       //Maybe works as a spot light?
        //overallPointLighting *= spotFactor;
    }
    
    /*
    vector from surface to light
    normalize vector
    distance from surface to light
    scale light by lamberts cosine law
    attenuate light by distance
    scale by spotlight
    */
    
	///Obtain overall final lighting for pixel
    float3 finalLight = ambientLight + overallPointLighting;
	///Obtain overall final colour of pixel
	float3 finalColour = pixelColour * saturate(finalLight);
    return float4(finalColour, 1.0f);
}