#include "Lighting.h"

void Lighting::InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& ambientLightingColour, const float ambientLightingStrength,
	const DirectX::XMFLOAT3& dynamicLightingColour, const float dynamicLightingStrength, const DirectX::XMFLOAT3A& dynamicLightingPos)
{
	///Setup Ambient Lighting
	aLight.InitialiseLighting(pDevice);

	///Setup Point Light
	pLight.InitialiseLighting(pDevice);
}

void Lighting::RenderLighting(ID3D11DeviceContext* pDeviceCon)
{
	///Render Ambient Light
	aLight.RenderLighting(pDeviceCon);

	///Render Point Light
	pLight.RenderLighting(pDeviceCon);
}
