#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 1

#define MAX_LIGHTS 4

struct LightData
{
    float3 lightColour;     //12-bit
    float lightStrength;    //4-bit
    
    float3 lightPosition;   //12-bit
    float lightFalloffEnd;  //4-bit
    
    float3 lightDirection;  //12-bit
    float lightFalloffStart;//4-bit
    
    float3 padding;         //12-bit
    float lightSpotFactor;  //4-bit
};

struct TextureData
{
    float4 diffuseAlbedo;   //16-bit
    
    float3 fresnelEff;      //12-bit
    float roughness;        //4-bit
};

//Single buffer to store directional light, point lights and spot lights
cbuffer lightsBuffer : register(b0)
{
    LightData mLights[NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS];
    float4 ambientLighting;
    float3 eyePosition;
    float padding;
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

float3 ComputeFresnelEffect(float3 matFresnel, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));
    
    float f0 = 1.f - cosIncidentAngle;
    float3 reflectPerc = matFresnel + (1.f - matFresnel) * (f0 * f0 * f0 * f0 * f0);
    return reflectPerc;
    
    //Calculates the material reflect percentage using Schlick's Approximation: R = R0 + (1 - R0) * (1 - cos0)^5
    //R is the reflect percentage returned
    //cos0 is normal dot vectorToLightSource (angle between normal of surface and direction of light)
    //R0 is the materials fresnel values which is calculated using R0 = ((1 - n2) / (1 + n2))^2 where n2 is the given matFresnel values
}

float3 BlinnPhong(TextureData M, float3 lightStrength, float3 lightVec, float3 N, float3 toEye, float4 albedo)
{
    const float m = M.roughness * 256.f; //Shininess
    float3 halfVec = normalize(toEye + lightVec);
    
    float roughnessFactor = (m + 8.f) * pow(max(dot(halfVec, N), 0.f), m) / 8.f;
    float3 fresnelFactor = ComputeFresnelEffect(M.fresnelEff, halfVec, lightVec);
    
    float3 specAlbedo = fresnelFactor * roughnessFactor;
    
    specAlbedo = specAlbedo / (specAlbedo + 1.f);
    
    return (albedo.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLighting(LightData dirL, TextureData M, float3 N, float3 toEye, float4 albedo)
{
    //Light vector aims opposite the direction the light ray travels
    float3 lightVec = -dirL.lightDirection;
    
    //Scale down light by Lambert Cosine Law
    float NdotL = max(dot(lightVec, N), 0.f);
    float3 lightStrength = (dirL.lightColour * dirL.lightStrength) * NdotL; //Might be light colour or strength
    
    return BlinnPhong(M, lightStrength, lightVec, N, toEye, albedo);
}

float3 ComputePointLighting(LightData pL, TextureData M, float3 pos, float3 N, float3 toEye, float4 albedo)
{
    //Vector from the surface to the light
    float3 lightVec = pL.lightPosition - pos;
    
    //Distance from surface to light
    float d = length(lightVec);
    
    //Out of range test
    if(d > pL.lightFalloffEnd)
        return float3(0.f, 0.f, 0.f);
    
    lightVec /= d;
    
    //Scale down light by Lambert cosine law
    float NdotL = max(dot(lightVec, N), 0.f);
    float3 lightStrength = (pL.lightColour * pL.lightStrength) * NdotL; //Might be light colour or strength 
    
    //Attenuate light by distance
    float att = saturate((pL.lightFalloffEnd - d) / (pL.lightFalloffEnd - pL.lightFalloffStart));
    lightStrength *= att;
    
    return BlinnPhong(M, lightStrength, lightVec, N, toEye, albedo);
}

float3 ComputeSpotLighting(LightData sL, TextureData M, float3 pos, float3 N, float3 toEye, float4 albedo)
{
    //Vector from the surface to the light
    float3 lightVec = sL.lightPosition - pos;
    
    //Distance from surface to light
    float d = length(lightVec);
    
    //Out of range test
    if (d > sL.lightFalloffEnd)
        return 0.f;
    
    lightVec /= d;
    
    //Scale down light by Lambert cosine law
    float NdotL = max(dot(lightVec, N), 0.f);
    float3 lightStrength = sL.lightColour * NdotL; //Might be light colour or strength 
    
    //Attenuate light by distance
    float att = saturate((sL.lightFalloffEnd - d) / (sL.lightFalloffEnd - sL.lightFalloffStart));
    lightStrength *= att;
    
    //Scale by spotlight
    float spotFactor = pow(max(dot(-lightVec, sL.lightDirection), 0.f), sL.lightSpotFactor);
    lightStrength *= spotFactor;
    
    return BlinnPhong(M, lightStrength, lightVec, N, toEye, albedo);
}

float4 ComputeLighting(LightData pLight[MAX_LIGHTS], TextureData mat, float3 pos, float3 normal, float3 eyePos, float3 shadowFactor, float4 albedo)
{    
    float3 result = 0.f;
    
    int i = 0;
    
    for (i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
    {
        pLight[i].lightDirection = normalize(pLight[i].lightDirection);
        
        result += shadowFactor[i] * ComputeDirectionalLighting(pLight[i], mat, normal, eyePos, albedo);
    }
    
    for (i = NUM_DIRECTIONAL_LIGHTS; i < NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS; i++)
    {
        result += ComputePointLighting(pLight[i], mat, pos, normal, eyePos, albedo);
    }
    
    for (i = NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; i++)
    {
        pLight[i].lightDirection = normalize(pLight[i].lightDirection);
        
        result += ComputePointLighting(pLight[i], mat, pos, normal, eyePos, albedo);
    }
    
    return float4(result, 0.f);
}

float4 main(PixelInput data) : SV_TARGET
{
    float4 diffuseAlbedo = texData.diffuseAlbedo;
    float4 materialAlbedo = tex.Sample(samplerState, data.coords);      //Could add more samplerstates in the future
    
    //Combines the albedos together
    diffuseAlbedo *= materialAlbedo;
    
    //Renormalize the normal
    data.normals = normalize(data.normals);
    
    //Vector from point being lit to eye
    float3 toEye = normalize(eyePosition - data.worldPos.xyz);
    
    //Calculate Ambient Lighting
    float4 ambient = ambientLighting * diffuseAlbedo;
    
    const float shininess = 0.0001f;
    float3 shadowFactor = 1.f;
    float4 directLight = ComputeLighting(mLights, texData, data.worldPos.xyz, data.normals, toEye, shadowFactor, materialAlbedo);
    
    float4 litColour = lerp(diffuseAlbedo, directLight, diffuseAlbedo.a);
    
    litColour.a = diffuseAlbedo.a;
    
    return litColour;
    
    //maybe move albedo into a struct of some kind to pass to functions better
    //comment the code and remove any unneeded code
    
    //leave these comments somewhere in the GameObject class for future remembering:
    /*
    Higher fresnel values = brighter and more condensed light reflected back at the eye
    Lower fresnel values = lower and dimmed light reflected back at the eye
    
    Higher roughness = condensed reflection lighting
    Lower roughness = scattered reflective lighting
    */
}