#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "ConstantBufferData.h"
#include "ErrorMessaging.h"
#include "PointLight.h"
#include "AmbientLight.h"

class Lighting
{
public:
	///Function to setup buffer and variables for ambient lighting
	void InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& ambientLightingColour = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const float ambientLightingStrength = 1.f,
		const DirectX::XMFLOAT3& dynamicLightingColour = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const float dynamicLightingStrength = 1.f, const DirectX::XMFLOAT3A& dynamicLightingPos = DirectX::XMFLOAT3A(0.f, 0.f, 0.f));
	///Function called every frame to visually update lighting
	void RenderLighting(ID3D11DeviceContext* pDeviceCon);

	//Debug functions
	PointLight* const GetPointLight() { return &pLight; }
	AmbientLight* const GetAmbientLight() { return &aLight; }

private:
	AmbientLight aLight;
	PointLight pLight;
};