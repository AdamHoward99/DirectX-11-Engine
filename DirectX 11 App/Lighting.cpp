#include "Lighting.h"

void Lighting::InitialiseLighting(ID3D11Device* pDevice, const DirectX::XMFLOAT3& ambientLightingColour, const float ambientLightingStrength,
	const DirectX::XMFLOAT3& dynamicLightingColour, const float dynamicLightingStrength, const DirectX::XMFLOAT3A& dynamicLightingPos)
{
	///Create Lighting Buffer Desc
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof D3D11_BUFFER_DESC);

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof PS_CB_DATA;
	///Sets to 0 for Index and Vertex Buffers, Gives Write Access for Constant Buffer Only
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	HRESULT hr = pDevice->CreateBuffer(&bufferDesc, NULL, pLightingBuffer.GetAddressOf());
	///CreateBuffer(IN, OPTIONAL, OPTIONAL)
	///const D3D11_BUFFER_DESC* pDesc - Pointer to Buffer description for buffer
	///const D3D11_SUBRESOURCE_DATA* pInitialData - Pointer to subresource data struct describing init data
	///ID3D11Buffer** ppBuffer - Address of a pointer to the buffer object
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateBuffer()");

	///Set Ambient Lighting Variables
	lightingData.ambientLightingColour = ambientLightingColour;
	lightingData.ambientLightingStrength = ambientLightingStrength;

	///Set Dynamic Lighting Variables
	lightingData.dynamicLightingColour = dynamicLightingColour;
	lightingData.dynamicLightingStrength = dynamicLightingStrength;
	lightingData.dynamicLightPosition = dynamicLightingPos;
}

void Lighting::RenderLighting(ID3D11DeviceContext* pDeviceCon)
{
	///Set Constant Lighting Buffer
	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = pDeviceCon->Map(pLightingBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
	///Map(IN, IN, IN, OPTIONAL, OPTIONAL)
	///ID3D11Resource* pResource - Pointer to Constant Buffer
	///UINT Subresource			 - Subresource index number
	///D3D11_MAP MapType		 - Specifies CPU's read and write permissions for constant buffer
	///UINT MapFlags			 - Flag that specifies what CPU does when GPU is busy
	///D3D11_MAPPED_SUBRESOURCE* - Pointer to mapped struct for constant buffer resource

	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11DeviceContext::Map()");

	CopyMemory(mapRes.pData, &lightingData, sizeof PS_CB_DATA);
	pDeviceCon->Unmap(pLightingBuffer.Get(), NULL);
	///Unmap(IN, IN)
	///ID3D11Resource* pResource - Pointer to the constant buffer
	///UINT Subresource			 - index of a subresource which needs to be unmapped

	pDeviceCon->PSSetConstantBuffers(0, 1, pLightingBuffer.GetAddressOf());
	///VSSetConstantBuffers(IN, IN, OPTIONAL)
	///UINT StartSlot					- Starting index of array for constant buffer
	///UINT NumBuffers					- Number of constant buffers to set
	///ID3D11Buffer** ppConstantBuffers - Pointer to array of constant buffers to set
}

void Lighting::SetAmbientLightingColour(const DirectX::XMFLOAT3A& newColour)
{
	lightingData.ambientLightingColour = newColour;
}

void Lighting::SetAmbientLightingColour(const float colourX, const float colourY, const float colourZ)
{
	lightingData.ambientLightingColour = DirectX::XMFLOAT3A(colourX, colourY, colourZ);
}

void Lighting::SetAmbientLightingStrength(const float newStrength)
{
	lightingData.ambientLightingStrength = newStrength;
}

void Lighting::SetDynamicLightingColour(const DirectX::XMFLOAT3A& newColour)
{
	lightingData.dynamicLightingColour = newColour;
}

void Lighting::SetDynamicLightingColour(const float colourX, const float colourY, const float colourZ)
{
	lightingData.dynamicLightingColour = DirectX::XMFLOAT3A(colourX, colourY, colourZ);
}

void Lighting::SetDynamicLightingStrength(const float newStrength)
{
	lightingData.dynamicLightingStrength = newStrength;
}

void Lighting::SetDynamicLightingPosition(const DirectX::XMFLOAT3A& newPosition)
{
	lightingData.dynamicLightPosition = newPosition;
}

void Lighting::SetDynamicLightingPosition(const float posX, const float posY, const float posZ)
{
	lightingData.dynamicLightPosition = DirectX::XMFLOAT3A(posX, posY, posZ);
}
