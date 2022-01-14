#pragma once
#include "Light.h"

class SpotLight : public Light
{
public:
	void InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& lightColour = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const float lightStrength = 1.f) override;
	void RenderLighting(ID3D11DeviceContext* pDeviceCon) override;

	void SetLightColour(const DirectX::XMFLOAT3A& newColour) override;
	void SetLightColour(const float r, const float g, const float b) override;
	void SetLightStrength(const float newStrength) override;
	void SetLightPosition(const DirectX::XMFLOAT3A& newPosition) override;
	void SetLightPosition(const float colourX, const float colourY, const float colourZ) override;

	///XMFLOAT3A version of function to set the rotation of the lighting
	void SetLightRotation(const DirectX::XMFLOAT3A& newRotation);
	///Singular float version of function to set the rotation of the lighting
	void SetLightRotation(const float x, const float y, const float z);

	const int GetSpotLightNumber() const;

private:
	const void SetSpotLightNumber();
	SpLights lightData;
	///Coresponds to the number of the light in the above static struct variable
	int spotLightNo;
	///Incremented every time a point light is instantiated
	static int spotLightCount;
};
