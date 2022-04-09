#pragma once
#include "Shaders.h"
#include "ErrorMessaging.h"

//Class to mimic DirectX 12 PSO's allowing collection of Pixel Shader, Vertex Shader, Rasterizer State, Depth Stencil State, Blend State

class PSO
{
public:
	PSO(Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	PSO(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, const D3D11_BLEND_DESC& blendDesc, const D3D11_RASTERIZER_DESC& rasterDesc, const D3D11_DEPTH_STENCIL_DESC& DSDesc, 
		const D3D11_INPUT_ELEMENT_DESC inputLayout[], const UINT inputLayoutSize, const std::wstring& vShaderPath, const std::wstring& pShaderPath, const UINT DSRef = NULL);
	PSO(const PSO& otherPSO);	//Copy Constructor
	PSO(PSO&& otherPSO);		//Move Constructor

	ID3D11BlendState* GetBlendState() const;
	ID3D11RasterizerState* GetRasterizerState() const;
	ID3D11DepthStencilState* GetDSState() const;
	ID3D11InputLayout* GetInputLayout() const;
	const UINT GetDepthStencilRef() const;
	ID3D11PixelShader* GetPixelShader();
	ID3D11VertexShader* GetVertexShader();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	PixelShader pShader;
	VertexShader vShader;

	UINT depthStencilRef;

	//?
	//Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderView;
	//Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthView;
	//Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthBuffer;
};