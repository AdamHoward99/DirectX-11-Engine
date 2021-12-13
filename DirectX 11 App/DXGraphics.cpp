#include "DXGraphics.h"

UINT DXGraphics::ViewportCount = 0;

DXGraphics::~DXGraphics()
{
	delete spBatch;
	delete font;

	spBatch = nullptr;
	font = nullptr;
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

	//Initialize Timer TODO: in future use Utility class
	timer.Start();
	return true;
}

void DXGraphics::RenderFrame(Camera* const camera)
{
	float colour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderView.Get(), colour);
	pDeviceContext->ClearDepthStencilView(pDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//Render objects here

	//Set input layout, topology, shaders and vertex buffers
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

	//Updating Constant Buffers
	VS_CB_DATA data;

	//TODO: Add Transformations to image here
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
	
	data.pos = camera->GetCameraView() * camera->GetProjection();
	data.pos = DirectX::XMMatrixTranspose(data.pos);	///Notice: DirectX uses XZ Plane instead of XY Plane
	camera->LookAt(data.pos);		///If matrix is not transposed before being passed, transpose in function

	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = pDeviceContext->Map(pConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);

	CopyMemory(mapRes.pData, &data, sizeof VS_CB_DATA);
	pDeviceContext->Unmap(pConstantBuffer.Get(), NULL);
	pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	UINT stride = sizeof Vertex;
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	pDeviceContext->PSSetShaderResources(0, 1, pDefaultTexture.GetAddressOf());		//Sets the TEXTURE value in pixelShader.hlsl

	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, NULL);

	pDeviceContext->DrawIndexed(3, 0, 0);		//Indices to draw | Starting position of indices | Starting position of vertex

	//Display FPS Timer
	timer.IncrementFPSCounter();
	if (timer.GetMilliseconds() > 1000.0)		//After a second has passed
	{
		timer.SetFPSString();
		timer.Reset();
		timer.ResetFPSCounter();
	}

	DrawString();
	pSwapChain->Present(1, NULL);
}

bool DXGraphics::InitialiseDX(HWND hwnd, int w, int h)
{
	std::vector<Data> adapters = adapterReader.GetData();

	if (adapters.size() < 1)
		exit(-1);

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
		ErrorMes::DisplayErrMessage(hr);

	//Give pointer to Back Buffer using Swap Chain
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);

	//Create Render Target View
	hr = pDevice->CreateRenderTargetView(backBuffer.Get(), NULL, pRenderView.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);

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
		ErrorMes::DisplayErrMessage(hr);

	hr = pDevice->CreateDepthStencilView(pDepthBuffer.Get(), NULL, pDepthView.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);

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
		ErrorMes::DisplayErrMessage(hr);

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
		ErrorMes::DisplayErrMessage(hr);

	font = new DirectX::SpriteFont(pDevice.Get(), L"Fonts\\Arial_16.spritefont");
	spBatch = new DirectX::SpriteBatch(pDeviceContext.Get());

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
		ErrorMes::DisplayErrMessage(hr);

	return true;
}

bool DXGraphics::InitialiseShaders()
{
	//Create Vertex Shader
	if (!vShader.Initialise(pDevice, L"Debug\\VertexShader.cso"))		//TODO: Change when altering configurations
		return false;

	//Create Pixel Shader
	if (!pShader.Initialise(pDevice, L"Debug\\PixelShader.cso"))
		return false;

	//Create Input Layouts
	D3D11_INPUT_ELEMENT_DESC layouts[] = {
		{"POSITION", NULL, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", NULL, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HRESULT hr = pDevice->CreateInputLayout(layouts, ARRAYSIZE(layouts), vShader.GetShaderBuffer()->GetBufferPointer(), vShader.GetShaderBuffer()->GetBufferSize(), pInputLayout.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);

	return true;
}

bool DXGraphics::InitialiseScene(int w, int h)
{
	//Vertices should always be clockwise
	Vertex v[] = 
	{ 
		//Triangle List Example Vertices 1-3
		Vertex(DirectX::XMFLOAT3(-0.25f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex(DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(0.25f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		//Triangle Strip Example Vertices 4-7
		Vertex(DirectX::XMFLOAT3(0.0f, -0.2f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(0.25f, -0.2f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(0.1f, -0.4f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(0.35f, -0.4f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),

		//Individual Points Example 8-10
		Vertex(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(0.2f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(0.1f, -0.2f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),

		///NOTICE: Vertices with 0.1 Y-value don't show using current graphics card (NVIDIA GeForce GTX 1050)
	};

	DWORD indices[] =
	{
		0, 1, 2,
	};

	//Create Vertex Buffer
	CreateBuffer(D3D11_BIND_VERTEX_BUFFER, sizeof Vertex * ARRAYSIZE(v), pVertexBuffer.GetAddressOf(), v);		//Vertex Buffer
	CreateBuffer(D3D11_BIND_INDEX_BUFFER, sizeof DWORD * ARRAYSIZE(indices), pIndexBuffer.GetAddressOf(), indices);		//Index Buffer
	CreateBuffer(D3D11_BIND_CONSTANT_BUFFER, sizeof VS_CB_DATA, pConstantBuffer.GetAddressOf(), nullptr, D3D11_USAGE_DYNAMIC);	//Constant Buffer

	//Load in texture
	///Notice - If texture is empty, call CoInitialize(NULL) before this, typically in app main entry point
	HRESULT hr = DirectX::CreateWICTextureFromFile(pDevice.Get(), L"Textures\\defaultTexture.png", nullptr, pDefaultTexture.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);

	return true;
}

void DXGraphics::DrawString()
{
	spBatch->Begin();
	///Notice: Draw all strings to be outputted here
	font->DrawString(spBatch, timer.GetFPSString().c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f);
	spBatch->End();
}

template<typename T>
void DXGraphics::CreateBuffer(const int bindFlag, const UINT byteWidth, ID3D11Buffer** bufferPtr, const T& resourceData, const D3D11_USAGE bufferUsage)
{
	//Create Buffer Desc
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof D3D11_BUFFER_DESC);

	bufferDesc.BindFlags = bindFlag;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.CPUAccessFlags = (bufferUsage == D3D11_USAGE_DEFAULT) ? 0 : D3D11_CPU_ACCESS_WRITE;		//Sets to 0 for Index, Vertex Buffers, Gives Write Access for Constant Buffer
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = bufferUsage;

	HRESULT hr;

	if (resourceData != nullptr)
	{
		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof D3D11_SUBRESOURCE_DATA);
		bufferData.pSysMem = resourceData;
		hr = pDevice->CreateBuffer(&bufferDesc, &bufferData, bufferPtr);
	}
	else
		hr = pDevice->CreateBuffer(&bufferDesc, NULL, bufferPtr);

	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);
}

