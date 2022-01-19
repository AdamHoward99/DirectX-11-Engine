#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 1

struct LightData
{
    float3 lightColour;     //12-bit
    float lightStrength;    //4-bit
    float3 lightPosition;   //12-bit
    float lightFalloffEnd;  //4-bit
    float lightSpotFactor;  //4-bit
};

//Single buffer to store ambient light, point lights and spot lights
cbuffer lightsBuffer : register(b0)
{
    LightData mLights[1 + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS];
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

float3 ComputePointLighting(LightData pLight, PixelInput data)
{
    //Obtain vector from pixel to light
    float3 vectorToLightSource = pLight.lightPosition - data.worldPos.xyz;
    //Get Distance from pixel to light
    float distance = length(vectorToLightSource);
    //Check if light is out of range from pixel
    if (distance > pLight.lightFalloffEnd)
        return float3(0.f, 0.f, 0.f);
    //Normalize distance vector
    vectorToLightSource /= distance;
	//Get Diffuse light intensity
    float3 diffuseLightInt = max(dot(vectorToLightSource, data.normals), 0.f) * pLight.lightStrength;
    //attenuate light by distance
    float att = saturate((pLight.lightFalloffEnd - distance) / (pLight.lightFalloffEnd - 0.f));
    diffuseLightInt * att;
	///Calculate Diffuse lighting
    float3 diffuseLight = diffuseLightInt  * pLight.lightColour;
    return diffuseLight;
}

float3 ComputeSpotLighting(LightData spotlight, PixelInput data)
{
    //Obtain vector from pixel to light
    float3 vectorToLightSource = spotlight.lightPosition - data.worldPos.xyz;
    //Get Distance from pixel to light
    float distance = length(vectorToLightSource);
    //Check if light is out of range from pixel
    if (distance > spotlight.lightFalloffEnd)
        return float3(0.f, 0.f, 0.f);
    //Normalize distance vector
    vectorToLightSource /= distance;
    //Scale cone of light by the spotFactor
    float spotFactor = pow(max(dot(-vectorToLightSource, float3(0.f, -1.f, 0.f)), 0.f), spotlight.lightSpotFactor);     //Default position is -y (facing downwards)
	//Get Diffuse light intensity
    float3 diffuseLightInt = max(dot(vectorToLightSource, data.normals), 0.f) * spotlight.lightStrength * spotFactor;
    //attenuate light by distance
    float att = saturate((spotlight.lightFalloffEnd - distance) / (spotlight.lightFalloffEnd - 0.f));       //Defaults falloffStart to 0
    diffuseLightInt * att;
	//Calculate Diffuse lighting
    float3 diffuseLight = diffuseLightInt * spotlight.lightColour;
    return diffuseLight;
}

float4 main(PixelInput data) : SV_TARGET
{
	///Obtain colour of the texture
    float3 pixelColour = tex.Sample(samplerState, data.coords).xyz;
	///Obtain normal of the texture
	float3 normalColour = data.normals;
	///Obtain the ambient lighting
    float3 ambientLight = mLights[0].lightColour * mLights[0].lightStrength;
    
    int lightArrayOffset = 1;
    
    ///Stores combined equation of all point lights in scene
    float3 overallPointLighting;
    for (int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        overallPointLighting += ComputePointLighting(mLights[lightArrayOffset], data);
        lightArrayOffset++;
    }
    
    //Stores combined equation of all spotlights in scene
    float3 overallSpotLighting = float3(0.f, 0.f, 0.f);
    for (int j = 0; j < NUM_SPOT_LIGHTS; j++)
    {
        overallSpotLighting += ComputeSpotLighting(mLights[lightArrayOffset], data);
        lightArrayOffset++;
    }
    
	///Obtain overall final lighting for pixel
    float3 finalLight = ambientLight + overallPointLighting + overallSpotLighting;
	///Obtain overall final colour of pixel
	float3 finalColour = pixelColour * saturate(finalLight);
    return float4(finalColour, 1.0f);
}