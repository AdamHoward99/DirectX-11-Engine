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
    //Calculates the material reflect percentage using Schlick's Approximation: R = R0 + (1 - R0) * (1 - cos0)^5
    //R is the reflect percentage returned
    //cos0 is normal dot vectorToLightSource (angle between normal of surface and direction of light)
    //R0 is the materials fresnel values which is calculated using R0 = ((1 - n2) / (1 + n2))^2 where n2 is the given matFresnel values
    
    float R0 = pow((1.f - matFresnel) / (1.f + matFresnel), 2);
    float cos0 = saturate(dot(normal, vectorToLightSource)); //Could saturate this
    float R = R0 + (1.f - R0) * pow((1.f - cos0), 5);
    return R;
    
    
    //Old calculations
    float cosAngle = saturate(dot(normal, vectorToLightSource));
    
    float f0 = 1.0f - cosAngle;
    
    float3 reflectPercent = matFresnel + (1.f - matFresnel) * pow(f0, 5.f);
    return reflectPercent;
}

float3 ComputeLightEyeReflection(float3 lightIntensity, float3 vectorToLightSource, float3 normal, float3 toEye, TextureData mat, float3 albedo)
{
    //Calculates Shininess of the material
    const float shininess = mat.roughness * 256.0f;
    //Calculate half vector between light vector and view vector
    float3 halfVector = normalize(vectorToLightSource + toEye);
    //Calculates Roughness Factor for the material using (m + 8) / 8 * (n dot hv) ^ m 
    //m is the Shininess of the material
    //n is the surface normal
    //hv is the vector halfway between light and eye positions (halfVector)
    float3 roughnessFactor = ((shininess + 8.f) / 8.f) * pow(max(dot(halfVector, normal), 0.f), shininess); //max is used to prevent values going negative
   
    //Obtain Fresnel Factor
    float3 fresnelFactor = ComputeFresnelEffect(mat.fresnelEff, halfVector, vectorToLightSource);
    
    //Calculates Specular Lighting using max(lightSourceVec dot n, 0.f) dot lightIntensity
    //lightSourceVec is the vector of the light source (vectorToLightSource)
    //n is the surface normal
    //lightIntensity is the intensity of the direct light
    
    return ((roughnessFactor * fresnelFactor)) * lightIntensity;
    
    //Old Calculations
    //const float m = mat.roughness * 256.f;
    
    //float3 halfDistanceVector = normalize(vectorToLightSource + toEye); //to eye?
    
    //float roughnessFactor = (m + 8.f) * pow(max(dot(halfDistanceVector, normal), 0.f), m) / 8.f;
    
    //float3 fresnelFactor = ComputeFresnelEffect(mat.fresnelEff, halfDistanceVector, vectorToLightSource);
    
    //float3 specAlbedo = fresnelFactor * roughnessFactor;
    
    //specAlbedo = specAlbedo / (specAlbedo + 1.f);
    
    //return (albedo + specAlbedo) * lightIntensity;
}

float3 ComputeLighting(LightData pLight, PixelInput data, TextureData mat, float3 normal, float3 albedo)
{    
    //Obtain light vector from current pixel to the light source
    float3 lightVector = pLight.lightPosition - data.worldPos.xyz;
    
    //Get Distance from pixel to light
    float distance = length(lightVector);
    
    //Calculate Light Attenuation
    float att = saturate((pLight.lightFalloffEnd - distance) / (pLight.lightFalloffEnd - 0.f));
    
    //Check if light is out of range from pixel
    if (distance > pLight.lightFalloffEnd)
        return float3(0.f, 0.f, 0.f);
    
    //Normalize the light distance vector
    lightVector /= distance;
    
    //Calculate Diffuse Lighting
    //Cd = Kd dot Ld * Md
    //Kd = max(light vector, normal surface)
    //Md = material colour
    //Ld = diffuse light colour
    
    //Calculate Kd
    float3 Kd = max(dot(lightVector, normal), 0.f);
    
    //Calculate Diffuse Lighting
    float3 diffuseL = dot(Kd, pLight.lightColour) * albedo;
    diffuseL *= att;
    
    //Check if the light is using spotFactor (Spot Lights Only)
    float spotFactor = 1.f;
    if(pLight.lightSpotFactor != 0.f)
    {
        //Scale cone of light by the spotFactor
        spotFactor = pow(max(saturate(dot(-lightVector, float3(0.f, -1.f, 0.f))), 0.f), pLight.lightSpotFactor); //Default position is -y (facing downwards)
        diffuseL *= spotFactor;
    }
    
    //Calculate Specular Lighting
    //Ls = Ks * pow(saturate(dot(R,V)), a)
    //R = reflection vector
    //V = Eye position
    //a = material roughness
    
    float3 specularL = ComputeLightEyeReflection(pLight.lightStrength, lightVector, normal, mat.toEye, mat, albedo);

    //Final Lighting
    return (diffuseL + specularL) * pLight.lightColour;
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
    return float4(finalColour, 0.0f);
}