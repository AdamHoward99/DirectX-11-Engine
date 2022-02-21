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
	///XMFLOAT3A version of function to set the direction of the lighting, Directional / Spot lights only
	virtual void SetLightDirection(const DirectX::XMFLOAT3A& newDirection) {};
	///Singular floats version of function to set the direction of the lighting, Directional / Spot lights only
	virtual void SetLightDirection(const float x, const float y, const float z) {};
	///Obtains light number which corresponds to alignment in array of lights in mLightdata struct.
	virtual const int GetLightNumber() const { return 0; };
	///Sets the light number for the array alignment
	virtual const void SetLightNumber() {};
	///Function to set the falloffStart of the lighting, used for point / spot lights
	virtual const void SetLightFalloffStart(const float newStart) {};
	///Function to set the falloffEnd of the lighting, used for point / spot lights
	virtual const void SetLightFalloffEnd(const float newEnd) {};
	///Function to set the spotFactor of the lighting, determines the size of the light circle, lower value is wider and higher value is more concentrated, used for spot lights
	virtual const void SetLightSpotFactor(const float newFactor) {};
	///Function to set the ambient lighting of the scene, ambient lighting is seperate from all other types of lights
	const void SetAmbientLighting(const DirectX::XMFLOAT4A& aLighting) { mLightdata.ambientLighting = aLighting; }
	///Function to set the eye position of which reflections from the material are viewed from, correlates to the camera's position
	const void SetEyePosition(const DirectX::XMFLOAT3A& eyePos) { mLightdata.eyePosition = eyePos; }
	///Function to toggle the use of a fog effect in the scene
	const void SetFogEffectStatus(const bool cond) { mLightdata.fogEnabled = cond; }
	///Function to set the colour of the fog effect using the XMFLOAT4 type
	const void SetFogColour(const DirectX::XMFLOAT4A& fogCol) { mLightdata.fogColour = fogCol; }
	///Function to set the colour of the fog effect using individual floats
	const void SetFogColour(const float R, const float G, const float B, const float A) { mLightdata.fogColour = DirectX::XMFLOAT4A(R, G, B, A); }
	///Function to set the start position of the fog effect from the eye position (Camera position)
	const void SetFogStart(const float fogS) { mLightdata.fogStart = fogS; }
	///Function to set the range of the fog effect from the fog start position
	const void SetFogRange(const float fogR) { mLightdata.fogRange = fogR; }

	//bool for enable fog?

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pLightBuffer;
	///Values of all lights used in scene is carried across all instances so they can be used in a single register in PixelShader.hlsl.
	static Lights mLightdata;
	///Incremented every time a point light is instantiated
	static int totalLightCount;
	///Coresponds to the number of the light in the above static struct variable
	int lightNumber;

	/*
	Order of lights being created in the mLightdata struct follow:
	Directional Lights -> Point Lights -> Spot Lights,
	with 0 + NUM_DIRECTIONAL_LIGHTS, NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS for point lights and total of [NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS] + NUM_SPOT_LIGHTS being for spot lights.
	*/
};
