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
	if (!InitialiseScene())
		return false;

	return true;
}

void DXGraphics::RenderFrame()
{
	float colour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderView.Get(), colour);

	//Render objects here

	//Set input layout, topology, shaders and vertex buffers
	pDeviceContext->IASetInputLayout(pInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->RSSetState(pRasterizerState.Get());

	//D3D10_PRIMITIVE_TOPOLOGY_POINTLIST - Singular Vertices
	//D3D10_PRIMITIVE_TOPOLOGY_LINELIST - Connects 2 Vertices to form a line
	//D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP - Connects all Vertices into lines in order
	//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST - Makes triangle from 3 Vertices, needs to be in clockwise order
	//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP - Makes triangle from all Vertices in order, minimizes amount of vertices needed

	pDeviceContext->VSSetShader(vShader.GetVertexShader(), NULL, 0);
	pDeviceContext->PSSetShader(pShader.GetPixelShader(), NULL, 0);

	UINT stride = sizeof Vertex;
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	pDeviceContext->Draw(3, 0);		//Indices to draw | Starting position of indices

	//Example Text Drawing	TODO CREATE FUNCTION FOR THIS IN FUTURE
	spBatch->Begin();
	font->DrawString(spBatch, L"DEFAULT TEXT", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f);
	spBatch->End();

	pSwapChain->Present(1, NULL);
}

bool DXGraphics::InitialiseDX(HWND hwnd, int w, int h)
{
	std::vector<Data> adapters = adapterReader.GetData();

	if (adapters.size() < 1)
		exit(-1);		//TODO:CHANGE TO ERROR MESSAGE

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
	{
		//TODO: ERROR MESSAGE
		return false;
	}

	//Give pointer to Back Buffer using Swap Chain
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	if (FAILED(hr))
		return false;

	//Create Render Target View
	hr = pDevice->CreateRenderTargetView(backBuffer.Get(), NULL, pRenderView.GetAddressOf());
	if (FAILED(hr))
		return false;

	//Set Render Target
	pDeviceContext->OMSetRenderTargets(1, pRenderView.GetAddressOf(), NULL);

	//Set Rasterizer
	D3D11_VIEWPORT deviceViewport;
	ZeroMemory(&deviceViewport, sizeof D3D11_VIEWPORT);

	deviceViewport.Height = (FLOAT)h;
	deviceViewport.Width = (FLOAT)w;
	deviceViewport.TopLeftX = 0;
	deviceViewport.TopLeftY = 0;
	ViewportCount++;

	pDeviceContext->RSSetViewports(ViewportCount, &deviceViewport);

	//Create Rasterizer State
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof D3D11_RASTERIZER_DESC);
	
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	hr = pDevice->CreateRasterizerState(&rasterizerDesc, pRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(hwnd, "CreateRasterizerState function failed. Closing Application", "Rasterizer State Error", MB_OK);
		exit(-1);
	}

	font = new DirectX::SpriteFont(pDevice.Get(), L"Fonts\\Arial_16.spritefont");
	spBatch = new DirectX::SpriteBatch(pDeviceContext.Get());

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
		{"POSITION", NULL, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOUR", NULL, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HRESULT hr = pDevice->CreateInputLayout(layouts, ARRAYSIZE(layouts), vShader.GetVertexBuffer()->GetBufferPointer(), vShader.GetVertexBuffer()->GetBufferSize(), pInputLayout.GetAddressOf());
	if (FAILED(hr))
		return false;

	return true;
}

bool DXGraphics::InitialiseScene()
{
	//Vertices should always be clockwise
	Vertex v[] = 
	{ 
		//Triangle List Example Vertices 1-3
		Vertex(DirectX::XMFLOAT2(-0.25f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT2(0.0f, 0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT2(0.25f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),

		//Triangle Strip Example Vertices 4-7
		Vertex(DirectX::XMFLOAT2(0.0f, -0.2f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT2(0.25f, -0.2f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT2(0.1f, -0.4f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT2(0.35f, -0.4f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)), 

		//Individual Points Example 8-10
		Vertex(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT2(0.2f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
		Vertex(DirectX::XMFLOAT2(0.1f, -0.2f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),

		///NOTICE: Vertices with 0.1 Y-value don't show using current graphics card (NVIDIA GeForce GTX 1050)
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof D3D11_BUFFER_DESC);

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof Vertex * ARRAYSIZE(v);
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	vertexBufferData.pSysMem = v;

	HRESULT hr = pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, pVertexBuffer.GetAddressOf());
	if (FAILED(hr))
		return false;

	return true;
}

