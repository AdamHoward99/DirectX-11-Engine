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

struct TextureData
{
    float3 fresnelEff;      //12-bit
    float roughness;        //4-bit
    float3 toEye;           //12-bit
};

//Single buffer to store ambient light, point lights and spot lights
cbuffer lightsBuffer : register(b0)
{
    LightData mLights[1 + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS];
};

cbuffer materialBuffer : register(b1)
{
    TextureData texData;
}

SamplerState samplerState : SAMPLER : register(s0);
Texture2D tex : register(t0);

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 coords : TEXCOORD;
	float3 normals : NORMAL;
    float4 worldPos : WORLD_POSITION;
};

float3 ComputeFresnelEffect(float3 matFresnel, float3 normal, float3 vectorToLightSource)
{
    //R0 = ( (n-1) / (n+1) )^2
    float cosAngle = saturate(dot(normal, vectorToLightSource));
    
    float f0 = 1.0f - cosAngle;
    
    float3 reflectPercent = matFresnel + (1.f - matFresnel) * pow(f0, 5.f);
    return reflectPercent;
}

float3 ComputeLightEyeReflection(float3 lightIntensity, float3 vectorToLightSource, float3 normal, float3 toEye, TextureData mat, float3 albedo)
{
    const float m = mat.roughness * 256.f;
    
    float3 halfDistanceVector = normalize(vectorToLightSource + toEye);     //to eye?
    
    float roughnessFactor = (m + 8.f) * pow(max(dot(halfDistanceVector, normal), 0.f), m) / 8.f;
    
    float3 fresnelFactor = ComputeFresnelEffect(mat.fresnelEff, halfDistanceVector, vectorToLightSource);
    
    float3 specAlbedo = fresnelFactor * roughnessFactor;
    
    specAlbedo = specAlbedo / (specAlbedo + 1.f);
    
    return (albedo + specAlbedo) * lightIntensity;
}

float3 ComputeLighting(LightData pLight, PixelInput data, TextureData mat, float3 normal, float3 albedo)
{
     //Obtain vector from pixel to light
    float3 vectorToLightSource = pLight.lightPosition - data.worldPos.xyz;
    
    //Get Distance from pixel to lightm
    float distance = length(vectorToLightSource);
    
    //Check if light is out of range from pixel
    if (distance > pLight.lightFalloffEnd)
        return float3(0.f, 0.f, 0.f);
    
    //Normalize distance vector
    vectorToLightSource /= distance;
    
    //Check if the light is using spotFactor (Spot Lights only)
    float spotFactor = 1.f;
    if(pLight.lightSpotFactor != 0.f)
    {
        //Scale cone of light by the spotFactor
        spotFactor = pow(max(dot(-vectorToLightSource, float3(0.f, -1.f, 0.f)), 0.f), pLight.lightSpotFactor); //Default position is -y (facing downwards)
    }
    
	//Get Diffuse light intensity
    float3 diffuseLightInt = max(dot(vectorToLightSource, data.normals), 0.f) * pLight.lightStrength * spotFactor;
    
    //attenuate light by distance
    float att = saturate((pLight.lightFalloffEnd - distance) / (pLight.lightFalloffEnd - 0.f));
    diffuseLightInt * att;
    
    float3 blinnPhong = ComputeLightEyeReflection(diffuseLightInt, vectorToLightSource, normal, mat.toEye, mat, albedo);

	///Calculate Diffuse lighting
    float3 diffuseLight = blinnPhong * pLight.lightColour;
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
        overallPointLighting += ComputeLighting(mLights[lightArrayOffset], data, texData, normalColour, pixelColour);
        lightArrayOffset++;
    }
    
    //Stores combined equation of all spotlights in scene
    float3 overallSpotLighting = float3(0.f, 0.f, 0.f);
    for (int j = 0; j < NUM_SPOT_LIGHTS; j++)
    {
        overallSpotLighting += ComputeLighting(mLights[lightArrayOffset], data, texData, normalColour, pixelColour);
        lightArrayOffset++;
    }
    
	///Obtain overall final lighting for pixel
    float3 finalLight = ambientLight + overallPointLighting + overallSpotLighting;
	///Obtain overall final colour of pixel
	float3 finalColour = pixelColour * saturate(finalLight);
    return float4(finalColour, 1.0f);
}