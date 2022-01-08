#pragma once
#include <DirectXMath.h>

struct VS_CB_DATA		///Notice: Needs to be 16-bit aligned to work
{
	DirectX::XMMATRIX pos;	//64-bit size
};

struct PS_CB_DATA		///Notice: Needs to be 16-bit aligned to work
{
	DirectX::XMFLOAT3 ambientLightingColour;	//12-bit
	float ambientLightingStrength;				//4-bit
};