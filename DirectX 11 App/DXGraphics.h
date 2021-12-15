#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "ErrorMessaging.h"
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "ConstantBufferData.h"
#include "Camera.h"
#include "FrameTimer.h"
#include "Object.h"
#include <SpriteBatch.h>
#include <spriteFont.h>
#include <WICTextureLoader.h>
#include <unordered_map>

class DXGraphics
{
public:
	bool InitialiseClass(HWND, int, int);
	~DXGraphics();

	void RenderFrame(Camera* const cam);
private:
	bool InitialiseDX(HWND, int, int);
	bool InitialiseShaders();
	bool InitialiseScene(int w, int h);
	void InitialiseOBJs();

	void DrawString();
	void LoadTextures();

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderView;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;

	//unordered map of objects which stores texture, buffers, indices, and vertices for each object
	std::unordered_map<std::string, std::unique_ptr<Object>> renderObjects;

	VertexShader vShader;
	PixelShader pShader;
	AdapterReader adapterReader;
	static UINT ViewportCount;

	DirectX::SpriteFont* font;
	DirectX::SpriteBatch* spBatch;
	//TODO: Create Text class to use fonts

	FrameTimer timer;
};
