#include "SpotLight.h"

void SpotLight::InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& lightColour, const float lightStrength)
{
	///Create Lighting Buffer Desc
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof D3D11_BUFFER_DESC);

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof Lights;
	///Sets to 0 for Index and Vertex Buffers, Gives Write Access for Constant Buffer Only
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	HRESULT hr = pDevice->CreateBuffer(&bufferDesc, NULL, pLightBuffer.GetAddressOf());
	///CreateBuffer(IN, OPTIONAL, OPTIONAL)
	///const D3D11_BUFFER_DESC* pDesc - Pointer to Buffer description for buffer
	///const D3D11_SUBRESOURCE_DATA* pInitialData - Pointer to subresource data struct describing init data
	///ID3D11Buffer** ppBuffer - Address of a pointer to the buffer object
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateBuffer()");

	///Set number of the point light, maximum value is stored in PixelShader.hlsl
	SetLightNumber();

	///Set Dynamic Lighting Variables
	mLightdata.mLights[lightNumber].lightColour = lightColour;
	mLightdata.mLights[lightNumber].lightStrength = lightStrength;
	mLightdata.mLights[lightNumber].lightPosition = DirectX::XMFLOAT3A(0.f, 0.f, 0.f);
	mLightdata.mLights[lightNumber].lightFalloffEnd = 10.f;
	mLightdata.mLights[lightNumber].lightSpotFactor = 10.f;
}

void SpotLight::RenderLighting(ID3D11DeviceContext* pDeviceCon)
{
	///Set Constant Lighting Buffer
	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = pDeviceCon->Map(pLightBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
	///Map(IN, IN, IN, OPTIONAL, OPTIONAL)
	///ID3D11Resource* pResource - Pointer to Constant Buffer
	///UINT Subresource			 - Subresource index number
	///D3D11_MAP MapType		 - Specifies CPU's read and write permissions for constant buffer
	///UINT MapFlags			 - Flag that specifies what CPU does when GPU is busy
	///D3D11_MAPPED_SUBRESOURCE* - Pointer to mapped struct for constant buffer resource

	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11DeviceContext::Map()");

	CopyMemory(mapRes.pData, &mLightdata, sizeof Lights);
	pDeviceCon->Unmap(pLightBuffer.Get(), NULL);
	///Unmap(IN, IN)
	///ID3D11Resource* pResource - Pointer to the constant buffer
	///UINT Subresource			 - index of a subresource which needs to be unmapped

	pDeviceCon->PSSetConstantBuffers(0, 1, pLightBuffer.GetAddressOf());
	///VSSetConstantBuffers(IN, IN, OPTIONAL)
	///UINT StartSlot					- Starting index of array for constant buffer
	///UINT NumBuffers					- Number of constant buffers to set
	///ID3D11Buffer** ppConstantBuffers - Pointer to array of constant buffers to set
}

void SpotLight::SetLightColour(const DirectX::XMFLOAT3A& newColour)
{
	mLightdata.mLights[lightNumber].lightColour = newColour;
}

void SpotLight::SetLightColour(const float r, const float g, const float b)
{
	mLightdata.mLights[lightNumber].lightColour = DirectX::XMFLOAT3A(r, g, b);
}

void SpotLight::SetLightStrength(const float newStrength)
{
	mLightdata.mLights[lightNumber].lightStrength = newStrength;
}

void SpotLight::SetLightPosition(const DirectX::XMFLOAT3A& newPosition)
{
	mLightdata.mLights[lightNumber].lightPosition = newPosition;
}

void SpotLight::SetLightPosition(const float posX, const float posY, const float posZ)
{
	mLightdata.mLights[lightNumber].lightPosition = DirectX::XMFLOAT3A(posX, posY, posZ);
}

void SpotLight::SetLightDirection(const DirectX::XMFLOAT3A& newDirection)
{
	mLightdata.mLights[lightNumber].lightDirection = newDirection;
}

void SpotLight::SetLightDirection(const float x, const float y, const float z)
{
	mLightdata.mLights[lightNumber].lightDirection = DirectX::XMFLOAT3A(x, y, z);
}

const void SpotLight::SetLightFalloffStart(const float newStart)
{
	mLightdata.mLights[lightNumber].lightFalloffStart = newStart;
}

const void SpotLight::SetLightFalloffEnd(const float newEnd)
{
	mLightdata.mLights[lightNumber].lightFalloffEnd = newEnd;
}

const void SpotLight::SetLightSpotFactor(const float newFactor)
{
	mLightdata.mLights[lightNumber].lightSpotFactor = newFactor;
}

const int SpotLight::GetLightNumber() const
{
	return lightNumber;
}

const void SpotLight::SetLightNumber()
{
	lightNumber = totalLightCount;
	totalLightCount++;
}
