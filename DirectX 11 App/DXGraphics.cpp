#include "DXGraphics.h"

UINT DXGraphics::ViewportCount = 0;

DXGraphics::~DXGraphics()
{
	fonts["default"]->~TextFont();
}

bool DXGraphics::InitialiseClass(HWND hwnd, int w, int h)
{
	if (!InitialiseDX(hwnd, w, h))
		return false;

	//Initialise shaders and scenes here
	if (!InitialiseShaders())
		return false;

	//Initialise the scene
	if (!InitialiseScene(w, h))
		return false;
	
	fTimer.StartTimer();
	return true;
}

void DXGraphics::RenderFrame(Camera* const camera, const float dt)
{
	//Update Ambient Lighting
	ambientLight.SetLightStrength(0.1f);

	//Update Point Light 0
	pointLights[0]->SetLightColour(DirectX::XMFLOAT3A(0.f, 0.f, 1.f));
	pointLights[0]->SetLightPosition(DirectX::XMFLOAT3A(0.f, 20.f, 0.f));

	pointLights[1]->SetLightColour(DirectX::XMFLOAT3A(0.f, 0.f, 0.f));
	pointLights[1]->SetLightPosition(DirectX::XMFLOAT3A(5.f, 4.f, 0.f));

	//Render lights
	ambientLight.RenderLighting(pDeviceContext.Get());

	///Point Lights only require a single light to call render since all point light data is passed across every instance (static)
	pointLights[0]->RenderLighting(pDeviceContext.Get());

	//Background
	float colour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderView.Get(), colour);
	pDeviceContext->ClearDepthStencilView(pDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//Set input layout, topology and shaders
	pDeviceContext->IASetInputLayout(pInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->RSSetState(pRasterizerState.Get());
	pDeviceContext->OMSetDepthStencilState(pDepthState.Get(), NULL);
	pDeviceContext->PSSetSamplers(NULL, 1, pSamplerState.GetAddressOf());

	//D3D10_PRIMITIVE_TOPOLOGY_POINTLIST - Singular Vertices
	//D3D10_PRIMITIVE_TOPOLOGY_LINELIST - Connects 2 Vertices to form a line
	//D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP - Connects all Vertices into lines in order
	//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST - Makes triangle from 3 Vertices, needs to be in clockwise order
	//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP - Makes triangle from all Vertices in order, minimizes amount of vertices needed

	pDeviceContext->VSSetShader(vShader.GetShader(), NULL, 0);
	pDeviceContext->PSSetShader(pShader.GetShader(), NULL, 0);

	///Notice: Update OBJ's here
	renderObjects["Ice_Cream"]->SetViewProjectionMatrix(camera->GetCameraView() * camera->GetProjection());
	renderObjects["Ice_Cream"]->SetRotation(0.f, 0.01f * dt, 0.f);
	renderObjects["Ice_Cream"]->Update();		///All transformations should be applied before this is called

	std::string txt = "Object X: " + std::to_string(renderObjects["Ice_Cream"]->GetRotationX()) + " Object Y: " + std::to_string(renderObjects["Ice_Cream"]->GetRotationY()) +
		" Object Z: " + std::to_string(renderObjects["Ice_Cream"]->GetRotationZ());

	std::string txt2 = "Camera X: " + std::to_string(camera->GetPosition().x) + " Camera Y: " + std::to_string(camera->GetPosition().y) + 
		" Camera Z: " + std::to_string(camera->GetPosition().z);

	fonts["default"]->DrawString(StringCon::StringToCString(txt), DirectX::XMFLOAT2A(0.f, 20.f));
	fonts["default"]->DrawString(StringCon::StringToCString(txt2), DirectX::XMFLOAT2A(0.f, 40.f));

	//Display FPS Timer
	fTimer.IncrementFPSCounter();
	if (fTimer.GetTimerMilliseconds() > 1000.0)		//After a second has passed
	{
		fTimer.SetFPSString();
		fTimer.RestartFrameTimer();
		fTimer.ResetFPSCounter();
	}

	DrawString();
	pSwapChain->Present(1, NULL);
}

bool DXGraphics::InitialiseDX(HWND hwnd, int w, int h)
{
	std::vector<Data> adapters = adapterReader.GetData();

	if (adapters.size() < 1)
		ErrorMes::DisplayErrorMessage(__LINE__, "DXGraphics::adapterReader: No adapters found", __FILE__);

	//Create Description for Swap Chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof DXGI_SWAP_CHAIN_DESC);

	swapChainDesc.BufferDesc.Width = w;
	swapChainDesc.BufferDesc.Height = h;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		NULL,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		this->pSwapChain.GetAddressOf(),
		this->pDevice.GetAddressOf(),
		NULL,
		this->pDeviceContext.GetAddressOf());

	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "D3D11CreateDeviceAndSwapChain()");

	//Give pointer to Back Buffer using Swap Chain
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "IDXGISwapChain::GetBuffer()");

	//Create Render Target View
	hr = pDevice->CreateRenderTargetView(backBuffer.Get(), NULL, pRenderView.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateRenderTargetView()");

	//Initialize Depth Stencil Buffer
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.Height = h;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.MiscFlags = 0;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.Width = w;

	hr = pDevice->CreateTexture2D(&depthTextureDesc, NULL, pDepthBuffer.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateTexture2D()");

	hr = pDevice->CreateDepthStencilView(pDepthBuffer.Get(), NULL, pDepthView.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateDepthStencilView()");

	//Set Render Target
	pDeviceContext->OMSetRenderTargets(1, pRenderView.GetAddressOf(), pDepthView.Get());

	//Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof D3D11_DEPTH_STENCIL_DESC);

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, pDepthState.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateDepthStencilState()");

	//Set Rasterizer
	D3D11_VIEWPORT deviceViewport;
	ZeroMemory(&deviceViewport, sizeof D3D11_VIEWPORT);

	deviceViewport.Height = (FLOAT)h;
	deviceViewport.Width = (FLOAT)w;
	deviceViewport.TopLeftX = 0;
	deviceViewport.TopLeftY = 0;
	deviceViewport.MinDepth = 0.f;
	deviceViewport.MaxDepth = 1.0f;
	ViewportCount++;

	pDeviceContext->RSSetViewports(ViewportCount, &deviceViewport);

	//Create Rasterizer State
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof D3D11_RASTERIZER_DESC);
	
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	hr = pDevice->CreateRasterizerState(&rasterizerDesc, pRasterizerState.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateRasterizerState()");

	///Note: Add fonts here
	fonts.insert({ "default", std::move(std::make_unique<TextFont>(pDevice.Get(), pDeviceContext.Get())) });

	//Create sampler desc
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof D3D11_SAMPLER_DESC);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0;

	hr = pDevice->CreateSamplerState(&samplerDesc, pSamplerState.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateSamplerState()");

	return true;
}

bool DXGraphics::InitialiseShaders()
{
	std::wstring vshaderFilepath;
	std::wstring pshaderFilepath;

#ifdef _DEBUG
	vshaderFilepath = L"x86\\Debug\\VertexShader.cso";
	pshaderFilepath = L"x86\\Debug\\PixelShader.cso";
#else
	vshaderFilepath = L"x86\\Release\\VertexShader.cso";
	pshaderFilepath = L"x86\\Release\\PixelShader.cso";
#endif

	//Create Vertex Shader
	if (!vShader.Initialise(pDevice, vshaderFilepath))
		return false;

	//Create Pixel Shader
	if (!pShader.Initialise(pDevice, pshaderFilepath))
		return false;

	//Create Input Layouts
	D3D11_INPUT_ELEMENT_DESC layouts[] = {
		{"POSITION", NULL, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", NULL, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   NULL, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HRESULT hr = pDevice->CreateInputLayout(layouts, ARRAYSIZE(layouts), vShader.GetShaderBuffer()->GetBufferPointer(), vShader.GetShaderBuffer()->GetBufferSize(), pInputLayout.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateInputLayout()");

	return true;
}

bool DXGraphics::InitialiseScene(int w, int h)
{
	InitialiseOBJs();
	InitialiseLighting();
	return true;
}

void DXGraphics::InitialiseOBJs()
{
	///Notice: Create OBJ's to be rendered in Scene here, Empty file name will give default triangle
	renderObjects["Ice_Cream"] = std::move(std::make_unique<GameObject>(pDevice, pDeviceContext, "OBJ/MultiObject/ice_cream.fbx"));
	renderObjects["Ice_Cream"]->SetRotation(-90.f, 0.f, 0.f);
}

void DXGraphics::InitialiseLighting()
{
	///Initialise Ambient Lighting linked to register b0
	ambientLight.InitialiseLighting(pDevice.Get());

	///Initialise Point Lighting linked to register b1
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		pointLights[i] = std::move(std::make_unique<PointLight>());
		pointLights[i]->InitialiseLighting(pDevice.Get());
	}
}

void DXGraphics::DrawString()
{
	///Note: Draw all strings to be outputted here
	fonts["default"]->DrawString(StringCon::StringToCString(fTimer.GetFPSString()));
}
