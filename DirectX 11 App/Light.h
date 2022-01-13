#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "ConstantBufferData.h"
#include "ErrorMessaging.h"

///All light types require these functions and thus inherit from this class
class Light
{
public:
	///Initialises ID3D11Buffer, colour and strength variables for light 
	virtual void InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& lightColour = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const float lightStrength = 1.f) = 0;
	///Displays light in scene
	virtual void RenderLighting(ID3D11DeviceContext* pDeviceCon) = 0;
	///XMFLOAT3A version of function to set the colour of the lighting
	virtual void SetLightColour(const DirectX::XMFLOAT3A& newColour) = 0;
	///Singular floats version of function to set the colour of the lighting
	virtual void SetLightColour(const float r, const float g, const float b) = 0;
	///Function to set the strength of the lighting
	virtual void SetLightStrength(const float newStrength) = 0;
	///XMFLOAT3A version of function to set the position of the lighting
	virtual void SetLightPosition(const DirectX::XMFLOAT3A& newPosition) {};
	///Singular floats version of function to set the position of the lighting
	virtual void SetLightPosition(const float x, const float y, const float z) {};

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pLightBuffer;
};
