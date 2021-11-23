#include "DXGraphics.h"

DXGraphics::~DXGraphics()
{}

bool DXGraphics::InitialiseClass(HWND hwnd, int w, int h)
{
	if (!InitialiseDX(hwnd, w, h))
		return false;

	//Initialise shaders and scenes here
	if (!InitialiseShaders())
		return false;

	return true;
}

void DXGraphics::RenderFrame()
{
	float colour[] = { 0.4f, 0.2f, 0.4f, 1.f };
	pDeviceContext->ClearRenderTargetView(pRenderView.Get(), colour);
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

	return true;
}

bool DXGraphics::InitialiseShaders()
{
	if (!vShader.Initialise(pDevice, L"Debug\\VertexShader.cso"))		//TODO: Change when altering configurations
		return false;

	D3D11_INPUT_ELEMENT_DESC layouts[] = { {"POSITION", NULL, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} };

	HRESULT hr = pDevice->CreateInputLayout(layouts, ARRAYSIZE(layouts), vShader.GetVertexBuffer()->GetBufferPointer(), vShader.GetVertexBuffer()->GetBufferSize(), pInputLayout.GetAddressOf());
	if (FAILED(hr))
		return false;

	return true;
}

