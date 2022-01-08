#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "ConstantBufferData.h"

class Lighting
{
public:
	///Constructor to setup buffer and variables for ambient lighting
	Lighting(ID3D11Device** pDevice, const DirectX::XMFLOAT3& ambientLightingColour = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const float ambientLightingStrength = 1.f);
	///Function called every frame to visually update lighting
	void RenderLighting(ID3D11DeviceContext** pDeviceCon);
	///XMFLOAT3A version of function to set the colour of the ambient lighting
	void SetAmbientLightingColour(const DirectX::XMFLOAT3A& newColour);
	///Singular floats version of function to set the colour of the ambient lighting
	void SetAmbientLightingColour(const float colourX, const float colourY, const float colourZ);
	///Function to set the strength of the ambient lighting
	void SetAmbientLightingStrength(const float newStrength);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pLightingBuffer;
	PS_CB_DATA lightingData;
};