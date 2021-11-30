#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include <SpriteBatch.h>
#include <spriteFont.h>

class DXGraphics
{
public:
	bool InitialiseClass(HWND, int, int);
	~DXGraphics();

	void RenderFrame();
private:
	bool InitialiseDX(HWND, int, int);
	bool InitialiseShaders();
	bool InitialiseScene();

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderView;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;

	VertexShader vShader;
	PixelShader pShader;
	AdapterReader adapterReader;
	static UINT ViewportCount;

	DirectX::SpriteFont* font;
	DirectX::SpriteBatch* spBatch;
};