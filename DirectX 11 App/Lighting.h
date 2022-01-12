#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "ConstantBufferData.h"
#include "ErrorMessaging.h"

class Lighting
{
public:
	///Function to setup buffer and variables for ambient lighting
	void InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& ambientLightingColour = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const float ambientLightingStrength = 1.f,
		const DirectX::XMFLOAT3& dynamicLightingColour = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const float dynamicLightingStrength = 1.f, const DirectX::XMFLOAT3A& dynamicLightingPos = DirectX::XMFLOAT3A(0.f, 0.f, 0.f));
	///Function called every frame to visually update lighting
	void RenderLighting(ID3D11DeviceContext* pDeviceCon);
	///XMFLOAT3A version of function to set the colour of the ambient lighting
	void SetAmbientLightingColour(const DirectX::XMFLOAT3A& newColour);
	///Singular floats version of function to set the colour of the ambient lighting
	void SetAmbientLightingColour(const float colourX, const float colourY, const float colourZ);
	///Function to set the strength of the ambient lighting
	void SetAmbientLightingStrength(const float newStrength);
	///XMFLOAT3A version of function to set the colour of the dynamic lighting
	void SetDynamicLightingColour(const DirectX::XMFLOAT3A& newColour);
	///Singular floats version of function to set the colour of the dynamic lighting
	void SetDynamicLightingColour(const float colourX, const float colourY, const float colourZ);
	///Function to set the strength of the dynamic lighting
	void SetDynamicLightingStrength(const float newStrength);
	///XMFLOAT3A version of function to set the position of the dynamic lighting
	void SetDynamicLightingPosition(const DirectX::XMFLOAT3A& newPosition);
	///Singular floats version of function to set the position of the dynamic lighting
	void SetDynamicLightingPosition(const float colourX, const float colourY, const float colourZ);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pLightingBuffer;
	PS_CB_DATA lightingData;
};