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

Texture::Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::string& filepath, aiTextureType texType)
{
	textureType = texType;
	
	///Get Image File Extension
	std::string extension = filepath.substr(filepath.find_last_of('.'));
	HRESULT hr;

	///Create Texture file using different functions dependant on if file is .dds or other
	if (extension == ".dds")
	{
		hr = DirectX::CreateDDSTextureFromFile(device.Get(), StringCon::WStringToCWString(StringCon::StringToWString(filepath)), texture.GetAddressOf(), textureSRV.GetAddressOf());
		///CreateDDSTextureFromFile(IN, IN, IN, IN, OPTIONAL, OPTIONAL)
		///ID3D11Device* d3dDevice - Pointer to device
		///const wchar_t* szFileName - C String of Image filename
		///ID3D11Resource** texture - Pointer to address of the texture resource which will store this image file
		///ID3D11ShaderResourceView** textureView - Pointer to address of the texture SRV
		///size_t maxsize - The maximum size of the texture resource, defaults to 0U
		///DDS_ALPHA_MODE* alphaMode - Pointer to ALPHA_MODE used for DDS file, defaults to nullptr

		if (FAILED(hr))
			InitializeColourTexture(device, &defaultColour, texType);

		return;
	}

	///Texture file is not .dds
	hr = DirectX::CreateWICTextureFromFile(device.Get(), StringCon::WStringToCWString(StringCon::StringToWString(filepath)), texture.GetAddressOf(), textureSRV.GetAddressOf());
	///CreateWICTextureFromFile(IN, IN, IN, IN, OPTIONAL)
	///ID3D11Device* d3dDevice				  - Pointer to the device using this resource
	///const wchar_t* fileName				  - C string pointer to texture file path
	///ID3D11Resource** texture				  - Pointer to the pointer of the texture resource 
	///ID3D11ShaderResourceView** textureView - Pointer to the pointer of the texture SRV
	///size_t maxsize						  - Maximum size of the texture file

	if (FAILED(hr))
		InitializeColourTexture(device, &defaultColour, texType);
}

Texture::Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const uint8_t * pData, size_t size, aiTextureType texType)
{
	textureType = texType;
	HRESULT hr = DirectX::CreateWICTextureFromMemory(device.Get(), pData, size, texture.GetAddressOf(), textureSRV.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "DirectX::CreateWICTextureFromMemory()");
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
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateTexture2D()");

	texture = pTex;

	///Create Shader Resource View for the texture
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureSRV.GetAddressOf());
	///CreateShaderResourceView(IN, OPTIONAL, OPTIONAL)
	///ID3D11Resource* pResource					- Pointer to the input shader resource
	///const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc - Pointer to the shader resource description
	///ID3D11ShaderResourceView** ppSRView			- Address of a pointer to a SRV
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateShaderResourceView()");
}
