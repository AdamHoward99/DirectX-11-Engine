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

SamplerState LinearWrapSS : SAMPLER : register(s0);
SamplerState PointWrapSS : SAMPLER : register(s1);
SamplerState AnisotropicWrapSS: SAMPLER : register(s2);

Texture2D tex : register(t0);

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 coords : TEXCOORD;
    float3 normals : NORMAL;
    float3 tangents : TANGENT;
    float4 worldPos : WORLD_POSITION;
};

float3 ComputeFresnelEffect(float3 matFresnel, float3 normal, float3 lightVec)
{
    //Calculates the material reflect percentage using Schlick's Approximation: R = R0 + (1 - R0) * (1 - cos0)^5
    //R is the reflect percentage returned
    //cos0 is angle between the normal of the surface and direction of the light (N�L)
    //R0 is the materials fresnel values
    float cos0 = 1.f - saturate(dot(normal, lightVec));
    float3 reflectPerc = matFresnel + (1.f - matFresnel) * pow(cos0, 5);
    return reflectPerc;
}

float3 BlinnPhong(TextureData M, float3 lightStrength, float3 lightVec, float3 N, float3 toEye, float4 albedo)
{
    //Calculates Shininess of material
    const float m = M.roughness * 256.f;
    float3 halfVec = normalize(toEye + lightVec);
    
    //Calculates material roughness using formula: (m + 8.f) * (H�N)^m / 8.f
    //m = Shininess of the material
    //H = Halfway vector between eye position and point being lit
    //N = Normal of surface
    float roughnessFactor = (m + 8.f) * pow(max(dot(halfVec, N), 0.f), m) / 8.f;
    float3 fresnelFactor = ComputeFresnelEffect(M.fresnelEff, halfVec, lightVec);
    
    //Obtains specular lighting for the material
    float3 specAlbedo = fresnelFactor * roughnessFactor;
    
    //Scale down for LDR rendering
    specAlbedo = specAlbedo / (specAlbedo + 1.f);
    
    return (albedo.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLighting(LightData dirL, TextureData M, float3 N, float3 toEye, float4 albedo)
{
    //Light vector aims opposite the direction the light ray travels
    float3 lightVec = -dirL.lightDirection;
    
    //Scale down light by Lambert Cosine Law: max(L�N, 0) * bL
    //L = Light vector aimed at the light source (-light direction)
    //N = Normal of the surface
    //bL = Amount of incoming direct light
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
    
    //Normalize the light vector
    lightVec /= d;
    
    //Scale down light by Lambert Cosine Law: max(L�N, 0) * bL
    //L = Light vector aimed at the light source (-light direction)
    //N = Normal of the surface
    //bL = Amount of incoming direct light
    float NdotL = max(dot(lightVec, N), 0.f);
    float3 lightStrength = (pL.lightColour * pL.lightStrength) * NdotL;
    
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
    
    //Normalize the light vector
    lightVec /= d;
    
    //Scale down light by Lambert Cosine Law: max(L�N, 0) * bL
    //L = Light vector aimed at the light source (-light direction)
    //N = Normal of the surface
    //bL = Amount of incoming direct light
    float NdotL = max(dot(lightVec, N), 0.f);
    float3 lightStrength = (sL.lightColour * sL.lightStrength) * NdotL;
    
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
    float3 totalLighting = 0.f;
    int i = 0;
    
    //Directional Lights
    for (i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
    {
        pLight[i].lightDirection = normalize(pLight[i].lightDirection);
        totalLighting += shadowFactor[i] * ComputeDirectionalLighting(pLight[i], mat, normal, eyePos, albedo);
    }
    
    //Point Lights
    for (i = NUM_DIRECTIONAL_LIGHTS; i < NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS; i++)
    {
        totalLighting += ComputePointLighting(pLight[i], mat, pos, normal, eyePos, albedo);
    }
    
    //Spot Lights
    for (i = NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; i++)
    {
        pLight[i].lightDirection = normalize(pLight[i].lightDirection);
        totalLighting += ComputePointLighting(pLight[i], mat, pos, normal, eyePos, albedo);
    }
    
    return float4(totalLighting, 0.f);
}

float4 main(PixelInput data) : SV_TARGET
{
    float4 diffuseAlbedo = texData.diffuseAlbedo;
    float4 materialAlbedo = tex.Sample(LinearWrapSS, data.coords); //Could add more samplerstates in the future
    
    //Combines the albedos together
    diffuseAlbedo *= materialAlbedo;
    
    //Renormalize the normal
    data.normals = normalize(data.normals);
    
    //Vector from point being lit to eye
    float3 toEye = normalize(eyePosition - data.worldPos.xyz);
    
    //Calculate Ambient Lighting using forumla: Ka = aL * mD
    //aL = the amount of incoming ambient light
    //mD = the diffuse albedo, specifies how much incoming light the surface reflects (Diffuse Reflectance)
    float4 ambient = ambientLighting * diffuseAlbedo;
    
    float3 shadowFactor = 1.f;
    
    //Returns total lighting from all Directional, Point and Spot lights enabled
    float4 directLight = ComputeLighting(mLights, texData, data.worldPos.xyz, data.normals, toEye, shadowFactor, materialAlbedo);
    
    float4 litColour = lerp(diffuseAlbedo, ambient + directLight, diffuseAlbedo.a);
    
    litColour.a = diffuseAlbedo.a;
    
    return litColour;
}