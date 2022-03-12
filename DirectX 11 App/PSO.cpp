#include "PSO.h"

PSO::PSO(Microsoft::WRL::ComPtr<ID3D11Device> pDevice)
{
	this->pDevice = pDevice;

	//Create Default Blend State
	CD3D11_BLEND_DESC blendDesc(D3D11_DEFAULT);
	HRESULT hr = pDevice->CreateBlendState(&blendDesc, pBlendState.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateBlendState()");

	//Create Default Rasterizer State
	CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
	hr = pDevice->CreateRasterizerState(&rasterizerDesc, pRasterizerState.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateRasterizerState()");

	//Create Default Depth Stencil State
	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, pDepthStencilState.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateDepthStencilState()");
}

PSO::PSO(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, const D3D11_BLEND_DESC& blendDesc, const D3D11_RASTERIZER_DESC& rasterDesc, const D3D11_DEPTH_STENCIL_DESC& DSDesc)
{
	this->pDevice = pDevice;

	//Create Blend State
	HRESULT hr = pDevice->CreateBlendState(&blendDesc, pBlendState.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateBlendState()");

	//Create Rasterizer State
	hr = pDevice->CreateRasterizerState(&rasterDesc, pRasterizerState.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateRasterizerState()");

	//Create Depth Stencil State
	hr = pDevice->CreateDepthStencilState(&DSDesc, pDepthStencilState.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateDepthStencilState()");
}

PSO::PSO(const PSO& otherPSO)
{
	this->pBlendState = otherPSO.pBlendState;
	this->pDepthStencilState = otherPSO.pDepthStencilState;
	this->pDevice = otherPSO.pDevice;
	this->pRasterizerState = otherPSO.pRasterizerState;
}

PSO::PSO(PSO&& otherPSO)
{
	this->pBlendState = otherPSO.pBlendState;
	this->pDepthStencilState = otherPSO.pDepthStencilState;
	this->pDevice = otherPSO.pDevice;
	this->pRasterizerState = otherPSO.pRasterizerState;

	//Deallocate otherPSO variables
	otherPSO.pBlendState = nullptr;
	otherPSO.pDepthStencilState = nullptr;
	otherPSO.pDevice = nullptr;
	otherPSO.pRasterizerState = nullptr;
}

ID3D11BlendState* PSO::GetBlendState() const
{
	return this->pBlendState.Get();
}

ID3D11RasterizerState* PSO::GetRasterizerState() const
{
	return this->pRasterizerState.Get();
}

ID3D11DepthStencilState* PSO::GetDSState() const
{
	return this->pDepthStencilState.Get();
}
