#include "Shaders.h"

bool VertexShader::Initialise(Microsoft::WRL::ComPtr<ID3D11Device> &pDevice, std::wstring shaderPathString)
{
	HRESULT hr = D3DReadFileToBlob(shaderPathString.c_str(), pVertexShaderBuffer.GetAddressOf());
	if (FAILED(hr))
		return false;

	hr = pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, pVertexShader.GetAddressOf());
	if (FAILED(hr))
		return false;

	return true;
}


bool PixelShader::Initialise(Microsoft::WRL::ComPtr<ID3D11Device> &pDevice, std::wstring shaderPathString)
{
	HRESULT hr = D3DReadFileToBlob(shaderPathString.c_str(), pPixelShaderBuffer.GetAddressOf());
	if (FAILED(hr))
		return false;

	hr = pDevice->CreatePixelShader(pPixelShaderBuffer.Get()->GetBufferPointer(), pPixelShaderBuffer.Get()->GetBufferSize(), NULL, pPixelShader.GetAddressOf());
	if (FAILED(hr))
		return false;

	return true;
}
