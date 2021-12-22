#include "Texture.h"
#include "ErrorMessaging.h"

Texture::Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const aiColor4D& colour, aiTextureType texType)
{
	InitializeColourTexture(device, &colour, texType);
}

Texture::Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const aiColor4D* colour, UINT w, UINT h, aiTextureType texType)
{
	InitializeColourTexture(device, colour, texType, w, h);
}

const aiTextureType Texture::GetTextureType() const
{
	return textureType;
}

const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::GetTextureRV() const
{
	return textureSRV;
}

void Texture::InitializeColourTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const aiColor4D* colourData, const aiTextureType texType, const UINT w, const UINT h)
{
	textureType = texType;
	///Creates structure description for texture, 8 bits used per colour value
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, w, h);
	Microsoft::WRL::ComPtr<ID3D11Resource> pTex = nullptr;
	D3D11_SUBRESOURCE_DATA initialData;
	initialData.pSysMem = colourData;
	initialData.SysMemPitch = w * sizeof aiColor4D;		///Size of a row of data

	///reinterpret_cast required as CreateTexture2D requires type of ID3D11Texture2D which inherits from ID3D11Resource
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, reinterpret_cast<ID3D11Texture2D**>(pTex.GetAddressOf()));
	///CreateTexture2D(IN, OPTIONAL, OPTIONAL)
	///const D3D11_TEXTURE2D_DESC* pDesc		  - Pointer to a description structure describing the 2D texture resource 
	///const D3D11_SUBRESOURCE_DATA* pInitialData - Pointer to array of Subresource data structures describing the 2D texture resource
	///ID3D11Texture2D** ppTexture2D			  - Pointer to buffer which receives the created textures information 
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);

	texture = pTex;

	///Create Shader Resource View for the texture
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureSRV.GetAddressOf());
	///CreateShaderResourceView(IN, OPTIONAL, OPTIONAL)
	///ID3D11Resource* pResource					- Pointer to the input shader resource
	///const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc - Pointer to the shader resource description
	///ID3D11ShaderResourceView** ppSRView			- Address of a pointer to a SRV
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);
}
