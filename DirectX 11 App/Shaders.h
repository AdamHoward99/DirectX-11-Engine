#pragma once
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

#include <string>

class VertexShader
{
public:
	bool Initialise(Microsoft::WRL::ComPtr<ID3D11Device>&, std::wstring);

	ID3D11VertexShader* GetVertexShader() { return pVertexShader.Get(); }
	ID3D10Blob* GetVertexBuffer() { return pVertexShaderBuffer.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> pVertexShaderBuffer;
};

class PixelShader		//TODO: Could create base class for these classes
{
public:
	bool Initialise(Microsoft::WRL::ComPtr<ID3D11Device>&, std::wstring);

	ID3D11PixelShader* GetPixelShader() { return pPixelShader.Get(); }
	ID3D10Blob* GetPixelBuffer() { return pPixelShaderBuffer.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> pPixelShaderBuffer;
};