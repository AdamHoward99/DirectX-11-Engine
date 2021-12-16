#include "Object.h"

Object::Object(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	pObjDevice = pDevice;
	pObjDeviceContext = pDeviceContext;
	Initialize();
}

Object::~Object()
{

}

void* Object::operator new(size_t i)
{
	return _mm_malloc(i, 16);		//Aligns to 16 for unique_ptr creation
}

void Object::operator delete(void * p)
{
	_mm_free(p);
}

void Object::Initialize()
{
	CreateObjBuffers();

}

void Object::Update()
{
	//Update Position of OBJ Here
	objData.pos = objWorldMatrix;
	Render();
}

void Object::Render()
{
	//Set Vertex of Obj
	UINT stride = sizeof Vertex;
	UINT offset = 0;
	pObjDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//Set Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = pObjDeviceContext->Map(pConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);

	CopyMemory(mapRes.pData, &objData, sizeof VS_CB_DATA);
	pObjDeviceContext->Unmap(pConstantBuffer.Get(), NULL);
	pObjDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	//Set Texture value in pixelShader.hlsl
	pObjDeviceContext->PSSetShaderResources(0, 1, pObjTexture.GetAddressOf());

	//Set Index Buffer of OBJ
	pObjDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, NULL);

	//Draw indices of OBJ
	pObjDeviceContext->DrawIndexed(18, 0, 0);		//Indices to draw | Starting position of indices | Starting position of vertex
}

void Object::LoadTexture(const std::wstring& texturePath)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(pObjDevice.Get(), texturePath.c_str(), nullptr, pObjTexture.GetAddressOf());

	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);
}

void Object::CreateObjBuffers()
{
	CreateExampleTriangle();
	CreateBuffer(D3D11_BIND_CONSTANT_BUFFER, sizeof VS_CB_DATA, pConstantBuffer.GetAddressOf(), nullptr, D3D11_USAGE_DYNAMIC);	//Constant Buffer
}

void Object::SetWorldPosition(const DirectX::XMMATRIX & pos)
{
	objWorldMatrix = pos;
	objWorldMatrix = DirectX::XMMatrixTranspose(objWorldMatrix);
}

const DirectX::XMMATRIX& Object::GetWorldPosition() const
{
	return objWorldMatrix;
}

void Object::CreateExampleTriangle()
{
	//Create Vertices and Indices for Obj	TODO: In future get this from file
	Vertex v[] =
	{
		//Triangle List Example Vertices 1-3
		//Front Face
		Vertex(DirectX::XMFLOAT3(-0.25f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex(DirectX::XMFLOAT3(0.0f, 0.5f, 0.25f), DirectX::XMFLOAT2(0.5f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(0.25f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		//Right Side
		Vertex(DirectX::XMFLOAT3(0.25f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex(DirectX::XMFLOAT3(0.0f, 0.5f, 0.25f), DirectX::XMFLOAT2(0.5f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(0.25f, 0.0f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		//Back Side
		Vertex(DirectX::XMFLOAT3(0.25f, 0.0f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex(DirectX::XMFLOAT3(0.0f, 0.5f, 0.25f), DirectX::XMFLOAT2(0.5f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(-0.25f, 0.0f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		//Left Side
		Vertex(DirectX::XMFLOAT3(-0.25f, 0.0f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex(DirectX::XMFLOAT3(0.0f, 0.5f, 0.25f), DirectX::XMFLOAT2(0.5f, 0.0f)),
		Vertex(DirectX::XMFLOAT3(-0.25f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		//Under Side
		Vertex(DirectX::XMFLOAT3(-0.25f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex(DirectX::XMFLOAT3(0.25f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex(DirectX::XMFLOAT3(-0.25f, 0.0f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		Vertex(DirectX::XMFLOAT3(0.25f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex(DirectX::XMFLOAT3(0.25f, 0.0f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex(DirectX::XMFLOAT3(-0.25f, 0.0f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		///NOTICE: Vertices with 0.1 Y-value don't show using current graphics card (NVIDIA GeForce GTX 1050)
		///NOTICE: Vertices should always be clockwise
	};

	DWORD indices[] =
	{
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,
		12, 13, 14,
		15, 16, 17
	};

	CreateBuffer(D3D11_BIND_VERTEX_BUFFER, sizeof Vertex * ARRAYSIZE(v), pVertexBuffer.GetAddressOf(), v);		//Vertex Buffer
	CreateBuffer(D3D11_BIND_INDEX_BUFFER, sizeof DWORD * ARRAYSIZE(indices), pIndexBuffer.GetAddressOf(), indices);		//Index Buffer
}

template<typename T>
void Object::CreateBuffer(const int bindFlag, const UINT byteWidth, ID3D11Buffer** bufferPtr, const T& resourceData, const D3D11_USAGE bufferUsage)
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
		hr = pObjDevice->CreateBuffer(&bufferDesc, &bufferData, bufferPtr);
	}
	else
		hr = pObjDevice->CreateBuffer(&bufferDesc, NULL, bufferPtr);
	
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);
}