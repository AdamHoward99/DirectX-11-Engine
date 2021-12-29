#include "Shaders.h"

bool VertexShader::Initialise(Microsoft::WRL::ComPtr<ID3D11Device> &pDevice, std::wstring shaderPathString)
{
	HRESULT hr = D3DReadFileToBlob(shaderPathString.c_str(), pVertexShaderBuffer.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, 5, "Shaders.cpp", "D3DReadFileToBlob()");

	hr = pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, pVertexShader.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, 9, "Shaders.cpp", "ID3D11Device::CreateVertexShader()");

	return true;
}


bool PixelShader::Initialise(Microsoft::WRL::ComPtr<ID3D11Device> &pDevice, std::wstring shaderPathString)
{
	HRESULT hr = D3DReadFileToBlob(shaderPathString.c_str(), pPixelShaderBuffer.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, 19, "Shaders.cpp", "D3DReadFileToBlob()");

	hr = pDevice->CreatePixelShader(pPixelShaderBuffer.Get()->GetBufferPointer(), pPixelShaderBuffer.Get()->GetBufferSize(), NULL, pPixelShader.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, 23, "Shaders.cpp", "ID3D11Device::CreatePixelShader()");

	return true;
}
