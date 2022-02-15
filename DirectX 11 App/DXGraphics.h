#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Camera.h"
#include "FrameTimer.h"
#include "GameObject.h"
#include "TextFont.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <unordered_map>

class DXGraphics
{
public:
	bool InitialiseClass(HWND, int, int);
	~DXGraphics();

	void RenderFrame(Camera* const cam, const float deltaTime);
private:
	bool InitialiseDX(HWND, int, int);
	bool InitialiseShaders();
	void CreateSamplerState(const int arrayOffset, const D3D11_TEXTURE_ADDRESS_MODE& textureAddress, const D3D11_COMPARISON_FUNC& comparisonFunc,
		const D3D11_FILTER& filter, const UINT maxAnisotropy = 1);
	bool InitialiseScene(int w, int h);
	void InitialiseMaterials();
	void InitialiseOBJs();
	void InitialiseLighting();

	void DrawString();


	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderView;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerStates[3];

	//unordered map of objects which stores texture, buffers, indices, and vertices for each object
	std::unordered_map<std::string, std::unique_ptr<GameObject>> renderObjects;

	VertexShader vShader;
	PixelShader pShader;
	AdapterReader adapterReader;
	static UINT ViewportCount;

	std::unordered_map<std::string, std::unique_ptr<TextFont>> fonts;

	FrameTimer fTimer;

	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;

	///Scene Lights
	DirectionalLight directionalLight;		//Todo: make this an unordered map
	std::unordered_map<int, std::unique_ptr<PointLight>> pointLights;
	std::unordered_map<int, std::unique_ptr<SpotLight>> spotLights;
};
