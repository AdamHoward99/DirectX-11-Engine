#include "Mesh.h"

Mesh::Mesh(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceCon,
	std::vector<Vertex>& v, std::vector<DWORD> i)
{
	pDevice = device;
	pDeviceContext = pDeviceCon;
	vertices = v;
	indices = i;
	CreateBuffers();		//Creates Vertex Buffer, Index Buffer, and Constant Buffer
}

Mesh::Mesh(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceCon)
{
	pDevice = device;
	pDeviceContext = pDeviceCon;
	CreateTriangleGeometry();
	CreateBuffers();		//Creates Vertex Buffer, Index Buffer, and Constant Buffer
}

//Mesh::Mesh(const Mesh& m)
//{
//	pDeviceContext = m.pDeviceContext;
//	pIndexBuffer = m.pIndexBuffer;
//	pVertexBuffer = m.pVertexBuffer;
//}

void Mesh::Draw()
{
	//Set Vertex of Obj
	UINT stride = sizeof Vertex;
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
	
	//Set Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = pDeviceContext->Map(pConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);

	CopyMemory(mapRes.pData, &meshData, sizeof VS_CB_DATA);
	pDeviceContext->Unmap(pConstantBuffer.Get(), NULL);
	pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	//Set Texture in PixelShader.hlsl
	pDeviceContext->PSSetShaderResources(0, 1, pTexture.GetAddressOf());
	
	//Set Index Buffer of OBJ
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, NULL);

	//Draw indices of OBJ
	pDeviceContext->DrawIndexed(indices.size(), 0, 0);		//Indices to draw | Starting position of indices | Starting position of vertex
}

void Mesh::CreateBuffers()
{
	CreateBuffer(D3D11_BIND_VERTEX_BUFFER, sizeof Vertex * vertices.size(), pVertexBuffer.GetAddressOf(), vertices);		//Vertex Buffer
	CreateBuffer(D3D11_BIND_INDEX_BUFFER, sizeof DWORD * indices.size(), pIndexBuffer.GetAddressOf(), indices);				//Index Buffer
	CreateBuffer(D3D11_BIND_CONSTANT_BUFFER, sizeof VS_CB_DATA, pConstantBuffer.GetAddressOf(), std::vector<int>(), D3D11_USAGE_DYNAMIC);	//Constant Buffer
}

void Mesh::UpdatePosition(const DirectX::XMMATRIX& worldMatrix)
{
	meshData.pos = DirectX::XMMatrixIdentity();
}

void Mesh::CreateTriangleGeometry()
{
	//Create Vertices and Indices for Created Triangle Example
	std::vector<Vertex> v =
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

	std::vector<DWORD> ind =
	{
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,
		12, 13, 14,
		15, 16, 17
	};

	vertices = v;
	indices = ind;
}

void Mesh::LoadTexture(const std::wstring & texturePath)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(pDevice.Get(), texturePath.c_str(), nullptr, pTexture.GetAddressOf());

	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);
}

template<typename T>
void Mesh::CreateBuffer(const int bindFlag, const UINT byteWidth, ID3D11Buffer** bufferPtr, const std::vector<T>& resourceData, const D3D11_USAGE bufferUsage)
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

	if (resourceData.size() <= 0)
	{
		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof D3D11_SUBRESOURCE_DATA);
		bufferData.pSysMem = &resourceData;
		hr = pDevice->CreateBuffer(&bufferDesc, &bufferData, bufferPtr);
	}
	else
		hr = pDevice->CreateBuffer(&bufferDesc, NULL, bufferPtr);

	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);
}
