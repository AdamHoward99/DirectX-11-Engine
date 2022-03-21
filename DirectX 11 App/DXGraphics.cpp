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
	//Update Directional Lighting
	directionalLights[0]->SetLightStrength(0.f);
	directionalLights[0]->SetEyePosition(camera->GetPosition());	//Required for reflected light from materials to be seen at eye position, any light type can call this

	//Update Point Lights
	pointLights[0]->SetLightColour(DirectX::XMFLOAT3A(1.f, 1.f, 1.f));
	pointLights[0]->SetLightPosition(DirectX::XMFLOAT3A(1.f, 3.f, -3.f));
	pointLights[0]->SetLightStrength(1.f);

	pointLights[1]->SetLightColour(DirectX::XMFLOAT3A(1.f, 1.f, 1.f));
	pointLights[1]->SetLightPosition(DirectX::XMFLOAT3A(3.5f, 3.f, -3.f));
	pointLights[1]->SetLightStrength(1.f);

	spotLights[0]->SetLightColour(DirectX::XMFLOAT3A(0.f, 0.f, 0.f));
	spotLights[0]->SetLightPosition(DirectX::XMFLOAT3A(0.f, 5.f, 0.f));

	//Create Fog Effect
	directionalLights[0]->SetFogColour(DirectX::XMFLOAT4A(0.35f, 0.35f, 0.35f, 1.f));
	directionalLights[0]->SetFogRange(10.f);
	directionalLights[0]->SetFogStart(10.f);
	directionalLights[0]->SetFogEffectStatus(false);

	//Render lights, only single light needs to be called to render since all light data is static across all lights
	directionalLights[0]->RenderLighting(pDeviceContext.Get());

	//Background
	float colour[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderView.Get(), colour);
	pDeviceContext->ClearDepthStencilView(pDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Set input layout, topology and shaders
	pDeviceContext->IASetInputLayout(pInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	///Note: Pixels closer to the camera need to be instantiated and drawn after objects which it is blending with
	///Note: Alternative is to draw all opaque objects first followed by any non-opaque after.
	pDeviceContext->PSSetSamplers(NULL, 2, pSamplerStates[0].GetAddressOf());

	//D3D10_PRIMITIVE_TOPOLOGY_POINTLIST - Singular Vertices
	//D3D10_PRIMITIVE_TOPOLOGY_LINELIST - Connects 2 Vertices to form a line
	//D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP - Connects all Vertices into lines in order
	//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST - Makes triangle from 3 Vertices, needs to be in clockwise order
	//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP - Makes triangle from all Vertices in order, minimizes amount of vertices needed

	pDeviceContext->VSSetShader(vShader.GetShader(), NULL, 0);
	pDeviceContext->PSSetShader(pShader.GetShader(), NULL, 0);

	///Notice: Update OBJ's here

	///Opaque Objects
	SetPSO(PSOs["Opaque"].get());

	renderObjects["marble"]->SetViewProjectionMatrix(camera->GetCameraView() * camera->GetProjection());
	renderObjects["marble"]->SetRotation(0.f, 0.01f * dt, 0.f);
	renderObjects["marble"]->SetMaterialFresnel(0.2f, 0.2f, 0.2f);
	renderObjects["marble"]->SetMaterialRoughness(0.3f);
	renderObjects["marble"]->Update();

	renderObjects["cobblestone"]->SetViewProjectionMatrix(camera->GetCameraView() * camera->GetProjection());
	renderObjects["cobblestone"]->SetRotation(0.f, 0.01f * dt, 0.f);
	renderObjects["cobblestone"]->SetMaterialFresnel(0.01f, 0.01f, 0.01f);
	renderObjects["cobblestone"]->SetMaterialRoughness(0.01f);
	renderObjects["cobblestone"]->Update();

	renderObjects["floor"]->SetViewProjectionMatrix(camera->GetCameraView() * camera->GetProjection());
	renderObjects["floor"]->Update();

	renderObjects["wall"]->SetViewProjectionMatrix(camera->GetCameraView() * camera->GetProjection());
	renderObjects["wall"]->SetMaterialFresnel(0.01f, 0.01f, 0.01f);
	renderObjects["wall"]->SetMaterialRoughness(0.01f);
	renderObjects["wall"]->Update();

	renderObjects["wall2"]->SetViewProjectionMatrix(camera->GetCameraView() * camera->GetProjection());
	renderObjects["wall2"]->SetMaterialFresnel(0.01f, 0.01f, 0.01f);
	renderObjects["wall2"]->SetMaterialRoughness(0.01f);
	renderObjects["wall2"]->Update();

	///Transparent Objects
	SetPSO(PSOs["Transparent"].get());
	 
	renderObjects["ice"]->SetViewProjectionMatrix(camera->GetCameraView() * camera->GetProjection());
	renderObjects["ice"]->SetRotation(0.f, 0.01f * dt, 0.f);
	renderObjects["ice"]->SetMaterialFresnel(0.01f, 0.01f, 0.01f);
	renderObjects["ice"]->SetMaterialRoughness(0.1f);
	renderObjects["ice"]->Update();		///All transformations should be applied before this is called
	

	//Note: Mirror code starts here

	//pDeviceContext->OMSetBlendState(pMirrorBlendState.Get(), NULL, 0xFFFFFFFF);
	//pDeviceContext->OMSetDepthStencilState(pDepthStencilStates[1].Get(), 1);

	renderObjects["Mirror"]->SetViewProjectionMatrix(camera->GetCameraView() * camera->GetProjection());
	renderObjects["Mirror"]->Update();

	////Get reflected position
	//DirectX::XMVECTOR mirrorPlane = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);		//xy plane
	//DirectX::XMMATRIX reflectMat = DirectX::XMMatrixReflect(mirrorPlane);
	//DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4A(&renderObjects["ice"]->GetTransformations4X4()) * reflectMat;

	//pDeviceContext->RSSetState(pRasteriserCullClockwise.Get());
	//pDeviceContext->OMSetDepthStencilState(pDepthStencilStates[2].Get(), 1);

	//renderObjects["ice"]->Update();

	////Reset back
	//pDeviceContext->RSSetState(pRasterizerState.Get());
	//pDeviceContext->OMSetDepthStencilState(pDepthStencilStates[0].Get(), 0);

	//pDeviceContext->OMSetBlendState(pBlendState.Get(), NULL, 0xFFFFFFFF);
	//renderObjects["Mirror"]->Update();


	std::string txt = "Object X: " + std::to_string(renderObjects["ice"]->GetRotationX()) + " Object Y: " + std::to_string(renderObjects["ice"]->GetRotationY()) +
		" Object Z: " + std::to_string(renderObjects["ice"]->GetRotationZ());

	fonts["default"]->DrawString(StringCon::StringToCString(txt), DirectX::XMFLOAT2A(0.f, 20.f));

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

void DXGraphics::SetPSO(const PSO* pso)
{
	//Set Blend State
	pDeviceContext->OMSetBlendState(pso->GetBlendState(), NULL, 0xFFFFFFFF);

	//Set Rasterizer State
	pDeviceContext->RSSetState(pso->GetRasterizerState());

	//Set Depth Stencil State
	pDeviceContext->OMSetDepthStencilState(pso->GetDSState(), pso->GetDepthStencilRef());
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
	
	///Initialise all engine PSO's
	InitialisePSOs();

	///Note: Add fonts here
	fonts.insert({ "default", std::move(std::make_unique<TextFont>(pDevice.Get(), pDeviceContext.Get())) });

	//Create Linear Wrap Sampler State (Default Used)
	CreateSamplerState(0, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);

	//Create Point Wrap Sampler State
	CreateSamplerState(1, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_POINT);

	//Create Anisotropic Wrap Sampler State
	CreateSamplerState(2, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER, D3D11_FILTER_ANISOTROPIC, 16);
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
		{"TANGENT",  NULL, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = pDevice->CreateInputLayout(layouts, ARRAYSIZE(layouts), vShader.GetShaderBuffer()->GetBufferPointer(), vShader.GetShaderBuffer()->GetBufferSize(), pInputLayout.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateInputLayout()");

	return true;
}

void DXGraphics::CreateSamplerState(const int arrayOffset, const D3D11_TEXTURE_ADDRESS_MODE& textureAddress, const D3D11_COMPARISON_FUNC& comparisonFunc,
	const D3D11_FILTER& filter, const UINT maxAnisotropy)
{
	//Create sampler desc
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof D3D11_SAMPLER_DESC);

	samplerDesc.AddressU = textureAddress;
	samplerDesc.AddressV = textureAddress;
	samplerDesc.AddressW = textureAddress;
	samplerDesc.ComparisonFunc = comparisonFunc;
	samplerDesc.Filter = filter;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxAnisotropy = maxAnisotropy;

	HRESULT hr = pDevice->CreateSamplerState(&samplerDesc, pSamplerStates[arrayOffset].GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateSamplerState()");
}

bool DXGraphics::InitialiseScene(int w, int h)
{
	InitialiseMaterials();
	InitialiseOBJs();
	InitialiseLighting();
	return true;
}

void DXGraphics::InitialiseMaterials()
{
	//Create Default Material
	mMaterials["Default"] = std::make_unique<Material>();
	mMaterials["Default"]->matName = "Default Material";
	mMaterials["Default"]->matData.matDiffuseAlbedo = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	mMaterials["Default"]->matData.matFresnelEffect = DirectX::XMFLOAT3A(0.01f, 0.01f, 0.01f);
	mMaterials["Default"]->matData.matRoughness = 0.01f;
	mMaterials["Default"]->matTextures[0] = std::move(Texture(pDevice, "OBJ/TexturedOBJExample/defaultTexture.png", aiTextureType_DIFFUSE));

	//Create Shadow Material
	mMaterials["Shadow"] = std::make_unique<Material>();
	mMaterials["Shadow"]->matName = "Shadow Material";
	mMaterials["Shadow"]->matData.matDiffuseAlbedo = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.5f);
}

void DXGraphics::InitialiseOBJs()
{
	///Notice: Create OBJ's to be rendered in Scene here, Empty file name will give default triangle
	renderObjects["ice"] = std::move(std::make_unique<GameObject>(pDevice, pDeviceContext, "OBJ/wicker.fbx"));
	renderObjects["ice"]->SetRotation(-90.f, 0.f, 0.f);
	//renderObjects["ice"]->AssignNewMaterial(*mMaterials["Default"]);

	renderObjects["marble"] = std::move(std::make_unique<GameObject>(pDevice, pDeviceContext, "OBJ/marble.fbx"));
	renderObjects["marble"]->SetRotation(-90.f, 0.f, 0.f);
	renderObjects["marble"]->SetPosition(3.f, 0.f, 0.f);

	renderObjects["cobblestone"] = std::move(std::make_unique<GameObject>(pDevice, pDeviceContext, "OBJ/cobblestone.fbx"));
	renderObjects["cobblestone"]->SetRotation(-90.f, 0.f, 0.f);
	renderObjects["cobblestone"]->SetPosition(6.f, 0.f, 0.f);

	//Default Scene OBJ's
	renderObjects["floor"] = std::move(std::make_unique<GameObject>(pDevice, pDeviceContext, "OBJ/floor.fbx"));
	renderObjects["floor"]->SetScale(12.f, 1.f, 12.f);
	renderObjects["floor"]->SetPosition(3.f, -1.f, 1.f);

	renderObjects["wall"] = std::move(std::make_unique<GameObject>(pDevice, pDeviceContext, "OBJ/wall.fbx"));
	renderObjects["wall"]->SetScale(12.f, 1.f, 12.f);
	renderObjects["wall"]->SetRotation(-90.f, 0.f, 0.f);
	renderObjects["wall"]->SetPosition(3.f, 5.f, 5.f);

	renderObjects["wall2"] = std::move(std::make_unique<GameObject>(pDevice, pDeviceContext, "OBJ/wall.fbx"));
	renderObjects["wall2"]->SetScale(12.f, 1.f, 12.f);
	renderObjects["wall2"]->SetRotation(-90.f, 90.f, 0.f);
	renderObjects["wall2"]->SetPosition(9.f, 5.f, 1.f);

	renderObjects["Mirror"] = std::move(std::make_unique<GameObject>(pDevice, pDeviceContext, "OBJ/mirror.fbx"));
	renderObjects["Mirror"]->SetScale(10.f, 1.f, 10.f);
	renderObjects["Mirror"]->SetRotation(-90.f, 90.f, 0.f);
	renderObjects["Mirror"]->SetPosition(-2.f, 1.f, 0.f);
}

void DXGraphics::InitialiseLighting()
{
	///Initialise Directional Lighting linked to register b0
	for (int i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		directionalLights[i] = std::move(std::make_unique<DirectionalLight>());
		directionalLights[i]->InitialiseLighting(pDevice.Get());
	}

	///Initialise Point Lighting linked to register b1
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		pointLights[i] = std::move(std::make_unique<PointLight>());
		pointLights[i]->InitialiseLighting(pDevice.Get());
	}

	for (int i = 0; i < NUM_SPOT_LIGHTS; i++)
	{
		spotLights[i] = std::move(std::make_unique<SpotLight>());
		spotLights[i]->InitialiseLighting(pDevice.Get());
	}
}

void DXGraphics::InitialisePSOs()
{
	/*
		PSO Settings for Opaque Objects
	*/

	///Create Opaque Blend State
	CD3D11_BLEND_DESC opaqueBlendDesc(D3D11_DEFAULT);

	///Create Opaque Rasterizer State
	D3D11_RASTERIZER_DESC opaqueRasterizerDesc;
	ZeroMemory(&opaqueRasterizerDesc, sizeof D3D11_RASTERIZER_DESC);
	opaqueRasterizerDesc.CullMode = D3D11_CULL_BACK;
	opaqueRasterizerDesc.FillMode = D3D11_FILL_SOLID;

	///Create Opaque Depth Stencil State
	CD3D11_DEPTH_STENCIL_DESC opaqueDDS(D3D11_DEFAULT);
	opaqueDDS.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	/* Default values of CD3D11_DEPTH_STENCIL_DESC:
	DepthEnable = TRUE,
	DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
	DepthFunc = D3D11_COMPARISON_LESS,
	StencilEnable = FALSE,
	StencilReadMask = 255,
	StencilWriteMask = 255,
	FrontFace && BackFace->StencilFunc = D3D11_COMPARISON_ALWAYS,
	FrontFace && BackFace->Operations = D3D11_STENCIL_OP_KEEP
	*/

	///Create Opaque PSO Settings
	PSOs["Opaque"] = std::move(std::make_unique<PSO>(pDevice, opaqueBlendDesc, opaqueRasterizerDesc, opaqueDDS));


	/*
		PSO Settings for Transparent Objects
	*/

	///Create Blend State
	D3D11_BLEND_DESC transparentBlendDesc;
	ZeroMemory(&transparentBlendDesc, sizeof D3D11_BLEND_DESC);

	D3D11_RENDER_TARGET_BLEND_DESC blendTarget;
	ZeroMemory(&blendTarget, sizeof D3D11_RENDER_TARGET_BLEND_DESC);

	///Setup Target Blend Description
	///Alpha Equation = Sa * Sbf (+/-) Da * Dbf
	///Sa  = Source Alpha
	///Sbf = Source Blend Factor
	///Da  = Destination Alpha
	///Dbf = Destination Blend Factor
	///(+/-) = Depends on the Blend Operation

	///Pixel Equation = Sp * Sbf (+/-) Dp * Dbf
	///Sp = Source Pixel
	///Dp = Destination Pixel

	blendTarget.BlendEnable = true;
	blendTarget.BlendOp = D3D11_BLEND_OP_ADD;
	blendTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	///Blend Operations:
	///D3D11_BLEND_OP_ADD - Adds together Source and Destination
	///D3D11_BLEND_OP_SUBTRACT - Subtracts Source from the Destination
	///D3D11_BLEND_OP_REV_SUBTRACT - Subtracts Destination from the Source
	///D3D11_BLEND_OP_MIN - Takes the lowest value of the Source and Destination
	///D3D11_BLEND_OP_MAX - Takes the highest value of the Source and Destination
	blendTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	blendTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
	///Blend Factors:
	///D3D11_BLEND_ZERO - Applies factor (0.f, 0.f, 0.f, 0.f)
	///D3D11_BLEND_ONE  - Applies factor (1.f, 1.f, 1.f, 1.f)
	///D3D11_BLEND_SRC_COLOR - Takes colour of source pixel (R, G, B, A)
	///D3D11_BLEND_INV_SRC_COLOR - Inverts above factor (1-R, 1-G, 1-B, 1-A)
	///D3D11_BLEND_SRC_ALPHA - Takes alpha of the source pixel (A, A, A, A)
	///D3D11_BLEND_INV_SRC_ALPHA - Inverts above factor (1-A, 1-A, 1-A, 1-A)
	///D3D11_BLEND_SRC_ALPHA_SAT - Clamps the source pixel alpha to 1 or less (f, f, f, 1) where f = min(A, 1-A)
	///D3D11_BLEND_BLEND_FACTOR - Uses factor set by ID3D11DeviceContext::OMSetBlendState()
	///D3D11_BLEND_INV_BLEND_FACTOR - Inverts above factor
	blendTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	///Specifying which components of the blending are writable:
	///D3D11_COLOR_WRITE_ENABLE_RED - Allows red to be visible
	///D3D11_COLOR_WRITE_ENABLE_GREEN - Allows green to be visible
	///D3D11_COLOR_WRITE_ENABLE_BLUE - Allows blue to be visible
	///D3D11_COLOR_WRITE_ENABLE_ALPHA - Allows the alpha to be visible
	///D3D11_COLOR_WRITE_ENABLE_ALL - Bitwise OR allowing all data to visible
	///These can be Bitwise OR together to create different blending results.
	transparentBlendDesc.RenderTarget[0] = blendTarget;

	///Create Transparent Rasterizer State
	D3D11_RASTERIZER_DESC transparentRasterizerDesc;
	ZeroMemory(&transparentRasterizerDesc, sizeof D3D11_RASTERIZER_DESC);
	transparentRasterizerDesc.CullMode = D3D11_CULL_NONE;
	transparentRasterizerDesc.FillMode = D3D11_FILL_SOLID;

	PSOs["Transparent"] = std::move(std::make_unique<PSO>(pDevice, transparentBlendDesc, transparentRasterizerDesc, opaqueDDS));

	/*
		PSO Settings for Mirrored Objects
	*/

	///Create Mirror Blend State
	CD3D11_BLEND_DESC mirrorBlendState(D3D11_DEFAULT);
	mirrorBlendState.RenderTarget->RenderTargetWriteMask = 0;

	///Create Mirror Depth Stencil State
	CD3D11_DEPTH_STENCIL_DESC mirrorDDS(D3D11_DEFAULT);
	mirrorDDS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	mirrorDDS.StencilEnable = true;
	mirrorDDS.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;


	PSOs["Mirrored"] = std::move(std::make_unique<PSO>(pDevice, mirrorBlendState, transparentRasterizerDesc, mirrorDDS, 1));

	/*
		PSO Settings for Reflected Objects
	*/

	///Create Reflected Rasterizer State
	D3D11_RASTERIZER_DESC reflectRasterizerDesc;
	ZeroMemory(&reflectRasterizerDesc, sizeof D3D11_RASTERIZER_DESC);
	reflectRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	reflectRasterizerDesc.CullMode = D3D11_CULL_BACK;
	reflectRasterizerDesc.FrontCounterClockwise = true;
	reflectRasterizerDesc.DepthClipEnable = true;

	///Create Reflected Depth Stencil State
	CD3D11_DEPTH_STENCIL_DESC reflectedDDS(D3D11_DEFAULT);
	reflectedDDS.StencilEnable = true;
	reflectedDDS.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	reflectedDDS.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	PSOs["Reflected"] = std::move(std::make_unique<PSO>(pDevice, opaqueBlendDesc, reflectRasterizerDesc, reflectedDDS, 1));

	/*
		PSO Settings for Shadowed Objects
	*/

	//Prevents Double Blending
	D3D11_DEPTH_STENCIL_DESC shadowBlendDesc;
	shadowBlendDesc.DepthEnable = true;
	shadowBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	shadowBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
	shadowBlendDesc.StencilEnable = true;
	shadowBlendDesc.StencilReadMask = 0xff;
	shadowBlendDesc.StencilWriteMask = 0xff;
	shadowBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	shadowBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	shadowBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	shadowBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	//Not Needed
	shadowBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	shadowBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	shadowBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	shadowBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	PSOs["Shadow"] = std::move(std::make_unique<PSO>(pDevice, transparentBlendDesc, transparentRasterizerDesc, shadowBlendDesc));
}

void DXGraphics::DrawString()
{
	///Note: Draw all strings to be outputted here
	fonts["default"]->DrawString(StringCon::StringToCString(fTimer.GetFPSString()));
}
