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
	void SetLightDirection(const DirectX::XMFLOAT3A& newDirection) override;
	void SetLightDirection(const float x, const float y, const float z) override;
	const void SetLightFalloffStart(const float newStart) override;
	const void SetLightFalloffEnd(const float newEnd) override;
	const void SetLightSpotFactor(const float newFactor) override;

	const int GetLightNumber() const override;

private:
	const void SetLightNumber() override;
};
