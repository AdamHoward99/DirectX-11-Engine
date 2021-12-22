#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <assimp/material.h>

class Texture
{
public:
	///Constructor to only take in a single colour value
	Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const aiColor4D& colour, aiTextureType texType);
	///Constructor to use multiple colours
	Texture(Microsoft::WRL::ComPtr<ID3D11Device> device, const aiColor4D* colour, UINT w, UINT h, aiTextureType texType);

	const aiTextureType GetTextureType() const;
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextureRV() const;

private:
	///Takes either a single colour value or multiple values, default parameters are 1 for single colour values.
	void InitializeColourTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const aiColor4D* colourData, const aiTextureType texType, const UINT w = 1, const UINT h = 1);

	Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV = nullptr;
	aiTextureType textureType = aiTextureType::aiTextureType_UNKNOWN;
};