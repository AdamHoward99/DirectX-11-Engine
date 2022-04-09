#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include "ErrorMessaging.h"
#include <string>

//Base class for both shaders
template <typename T>
class Shader
{
protected:
	virtual bool Initialise(Microsoft::WRL::ComPtr<ID3D11Device>&, std::wstring) { return false; }
	virtual T GetShader() { return nullptr; }
	virtual ID3D10Blob* GetShaderBuffer() { return nullptr; }
	virtual ~Shader() {};
};

//Vertex Shader Class
class VertexShader : public Shader<ID3D11VertexShader*>
{
public:
	bool Initialise(Microsoft::WRL::ComPtr<ID3D11Device>&, std::wstring) override;

	ID3D11VertexShader* GetShader() override { return pVertexShader.Get(); }
	ID3D10Blob* GetShaderBuffer() override { return pVertexShaderBuffer.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> pVertexShaderBuffer;
};

//Pixel Shader Class
class PixelShader : Shader<ID3D11PixelShader*>
{
public:
	bool Initialise(Microsoft::WRL::ComPtr<ID3D11Device>&, std::wstring) override;

	ID3D11PixelShader* GetShader() override { return pPixelShader.Get(); }
	ID3D10Blob* GetShaderBuffer() override { return pPixelShaderBuffer.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> pPixelShaderBuffer;
};