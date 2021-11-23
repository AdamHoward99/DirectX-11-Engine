#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include "AdapterReader.h"
#include "Shaders.h"

class DXGraphics
{
public:
	bool InitialiseClass(HWND, int, int);
	~DXGraphics();

	void RenderFrame();
private:
	bool InitialiseDX(HWND, int, int);
	bool InitialiseShaders();

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderView;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

	VertexShader vShader;
	AdapterReader adapterReader;
};