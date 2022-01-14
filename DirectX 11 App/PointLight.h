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

private:
	PointLightData lightData;
	int pointLightNo;
	static int pointLightCount;
};