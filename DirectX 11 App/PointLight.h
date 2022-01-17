#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	void InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& lightColour = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const float lightStrength = 1.f) override;
	void RenderLighting(ID3D11DeviceContext* pDeviceCon) override;

	void SetLightColour(const DirectX::XMFLOAT3A& newColour) override;
	void SetLightColour(const float r, const float g, const float b) override;
	void SetLightStrength(const float newStrength) override;
	void SetLightPosition(const DirectX::XMFLOAT3A& newPosition) override;
	void SetLightPosition(const float colourX, const float colourY, const float colourZ) override;

	///Function to set the falloffEnd of the point light
	void SetLightFalloffEnd(const float newEnd);

	const int GetPointLightNumber() const;

private:
	const void SetPointLightNumber();

	/*
	Values of all point lights used in scene is carried across all instances so they can be used in a single register in PixelShader.hlsl.
	Static data values also allows a single point light to render instead of all present in the scene.
	*/
	static PLights lightData;
	///Coresponds to the number of the light in the above static struct variable
	int pointLightNo;
	///Incremented every time a point light is instantiated
	static int pointLightCount;
};