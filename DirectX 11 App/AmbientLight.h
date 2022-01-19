#pragma once
#include "Light.h"

class AmbientLight : public Light
{
public:
	void InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& lightColour = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const float lightStrength = 1.f) override;
	void RenderLighting(ID3D11DeviceContext* pDeviceCon) override;

	void SetLightColour(const DirectX::XMFLOAT3A& newColour) override;
	void SetLightColour(const float r, const float g, const float b) override;
	void SetLightStrength(const float newStrength) override;
	const void SetLightFalloffEnd(const float newEnd) override;
	const void SetLightSpotFactor(const float newFactor) override;

	const int GetLightNumber() const override;

private:
	const void SetLightNumber() override;
};