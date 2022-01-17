#pragma once
#include <DirectXMath.h>
#define NUM_POINT_LIGHTS 2		///Coresponds to header defined in PixelShader.hlsl, number of point lights defined before scene creation
#define NUM_SPOT_LIGHTS 1		///Coresponds to header defined in PixelShader.hlsl, number of spot lights defined before scene creation

struct VS_CB_DATA		///Notice: Needs to be 16-bit aligned to work
{
	DirectX::XMMATRIX WVPMatrix;	//64-bit size
	DirectX::XMMATRIX WorldMatrix;	//64-bit size
};

struct AmbientLightData		///Notice: Needs to be 16-bit aligned to work
{
	DirectX::XMFLOAT3 ambientLightingColour;	//12-bit
	float ambientLightingStrength;				//4-bit
};

struct PointLightData
{
	DirectX::XMFLOAT3 dynamicLightingColour;	//12-bit
	float dynamicLightingStrength;				//4-bit
	DirectX::XMFLOAT3 dynamicLightPosition;		//12-bit
	float lightFalloffStart;					//4-bit
	float lightFalloffEnd;						//4-bit
	float padding;								//4-bit
};

struct SpotLightData
{
	DirectX::XMFLOAT3 dynamicLightingColour;	//12-bit
	float dynamicLightingStrength;				//4-bit
	DirectX::XMFLOAT3 dynamicLightPosition;		//12-bit
	float lightFalloffEnd;						//4-bit
	float spotFactor;							//4-bit
	DirectX::XMFLOAT3 padding;					//12-bit
};

struct PLights		///Notice: Needs to be 16-bit aligned to work
{
	///Allows multiple point lights to use a single register
	PointLightData lights[NUM_POINT_LIGHTS];
};

struct SpLights
{
	SpotLightData SPLights[NUM_SPOT_LIGHTS];
};
