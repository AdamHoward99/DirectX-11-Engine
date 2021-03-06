#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <assimp/material.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "StringConverter.h"

class Texture
{
public:
	///Default Constructor used for Material struct when without access to a ID3D11Device
	Texture() {};
	///Constructor to only take in a single colour value
	Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const aiColor4D& colour, aiTextureType texType);
	///Constructor to use multiple colours
	Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const aiColor4D* colour, UINT w, UINT h, aiTextureType texType);
	///Constructor to use Image files for textures
	Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::string& filepath, aiTextureType texType);
	///Constructor to use Image files for textures which are embedded & compressed
	Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const uint8_t* pData, size_t size, aiTextureType texType);

	const aiTextureType GetTextureType() const;
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextureRV() const;

private:
	///Takes either a single colour value or multiple values, default parameters are 1 for single colour values.
	void InitializeColourTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const aiColor4D* colourData, const aiTextureType texType, const UINT w = 1, const UINT h = 1);

	Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV = nullptr;
	aiTextureType textureType = aiTextureType::aiTextureType_UNKNOWN;
	aiColor4D defaultColour = aiColor4D(100.f, 100.f, 100.f, 255.f);
};