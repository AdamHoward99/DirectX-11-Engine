#pragma once
#include <DirectXMath.h>
#define NUM_POINT_LIGHTS 2		///Coresponds to header defined in PixelShader.hlsl, number of point lights defined before scene creation
#define NUM_SPOT_LIGHTS 1		///Coresponds to header defined in PixelShader.hlsl, number of spot lights defined before scene creation

struct VS_CB_DATA		///Notice: Needs to be 16-bit aligned to work
{
	DirectX::XMMATRIX WVPMatrix;	//64-bit size
	DirectX::XMMATRIX WorldMatrix;	//64-bit size
};
 
struct LightData		///Notice: Needs to be 16-bit aligned to work
{
	DirectX::XMFLOAT3 lightColour;		//12-bit
	float lightStrength;				//4-bit
	DirectX::XMFLOAT3 lightPosition;	//12-bit
	float lightFalloffEnd;				//4-bit
	float lightSpotFactor;				//4-bit
	DirectX::XMFLOAT3 padding;			//12-bit
};

struct Lights
{
	LightData mLights[1 + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS];
};

struct MaterialData
{
	DirectX::XMFLOAT3 matFresnelEffect;	//12-bit
	float matRoughness;					//4-bit
	DirectX::XMFLOAT3 toEye;			//12-bit
	float padding;						//4-bit
};

struct Material
{
	MaterialData matData;
};
