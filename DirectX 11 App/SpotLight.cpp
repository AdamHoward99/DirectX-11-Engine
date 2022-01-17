#include "SpotLight.h"

int SpotLight::spotLightCount = 0;
SpLights SpotLight::lightData;

void SpotLight::InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& lightColour, const float lightStrength)
{
	///Create Lighting Buffer Desc
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof D3D11_BUFFER_DESC);

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof SpLights;
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
	SetSpotLightNumber();

	///Set Dynamic Lighting Variables
	lightData.SPLights[spotLightNo].dynamicLightingColour = lightColour;
	lightData.SPLights[spotLightNo].dynamicLightingStrength = lightStrength;
	lightData.SPLights[spotLightNo].dynamicLightPosition = DirectX::XMFLOAT3A(0.f, 0.f, 0.f);
	lightData.SPLights[spotLightNo].lightFalloffEnd = 10.f;
	lightData.SPLights[spotLightNo].spotFactor = 10.f;
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

	CopyMemory(mapRes.pData, &lightData, sizeof SpLights);
	pDeviceCon->Unmap(pLightBuffer.Get(), NULL);
	///Unmap(IN, IN)
	///ID3D11Resource* pResource - Pointer to the constant buffer
	///UINT Subresource			 - index of a subresource which needs to be unmapped

	pDeviceCon->PSSetConstantBuffers(2, 1, pLightBuffer.GetAddressOf());
	///VSSetConstantBuffers(IN, IN, OPTIONAL)
	///UINT StartSlot					- Starting index of array for constant buffer
	///UINT NumBuffers					- Number of constant buffers to set
	///ID3D11Buffer** ppConstantBuffers - Pointer to array of constant buffers to set
}

void SpotLight::SetLightColour(const DirectX::XMFLOAT3A& newColour)
{
	lightData.SPLights[spotLightNo].dynamicLightingColour = newColour;
}

void SpotLight::SetLightColour(const float r, const float g, const float b)
{
	lightData.SPLights[spotLightNo].dynamicLightingColour = DirectX::XMFLOAT3A(r, g, b);
}

void SpotLight::SetLightStrength(const float newStrength)
{
	lightData.SPLights[spotLightNo].dynamicLightingStrength = newStrength;
}

void SpotLight::SetLightPosition(const DirectX::XMFLOAT3A& newPosition)
{
	lightData.SPLights[spotLightNo].dynamicLightPosition = newPosition;
}

void SpotLight::SetLightPosition(const float posX, const float posY, const float posZ)
{
	lightData.SPLights[spotLightNo].dynamicLightPosition = DirectX::XMFLOAT3A(posX, posY, posZ);
}

void SpotLight::SetLightFalloffEnd(const float newEnd)
{
	lightData.SPLights[spotLightNo].lightFalloffEnd = newEnd;
}

void SpotLight::SetLightSpotFactor(const float newFactor)
{
	lightData.SPLights[spotLightNo].spotFactor = newFactor;
}

const int SpotLight::GetSpotLightNumber() const
{
	return spotLightNo;
}

const void SpotLight::SetSpotLightNumber()
{
	spotLightNo = spotLightCount;
	spotLightCount++;
}
