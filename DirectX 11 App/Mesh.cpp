#include "Mesh.h"

Mesh::Mesh(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceCon,
	const std::vector<Vertex>& v, const std::vector<DWORD>& i)
{
	pDevice = device;
	pDeviceContext = pDeviceCon;
	CreateBuffers(v, i);		//Creates Vertex Buffer, Index Buffer, and Constant Buffer
}

Mesh::Mesh(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceCon)
{
	pDevice = device;
	pDeviceContext = pDeviceCon;
	CreateTriangleGeometry();
}

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

	//Set Index Buffer of OBJ
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, NULL);

	//Draw indices of OBJ
	pDeviceContext->DrawIndexed(indicesCount, 0, 0);		//Indices to draw | Starting position of indices | Starting position of vertex
}

void Mesh::CreateBuffers(const std::vector<Vertex>& v, const std::vector<DWORD>& i)
{
	CreateBuffer(D3D11_BIND_VERTEX_BUFFER, sizeof Vertex * v.size(), pVertexBuffer.GetAddressOf(), v.data());		//Vertex Buffer
	CreateBuffer(D3D11_BIND_INDEX_BUFFER, sizeof DWORD * i.size(), pIndexBuffer.GetAddressOf(), i.data());				//Index Buffer
	CreateBuffer(D3D11_BIND_CONSTANT_BUFFER, sizeof VS_CB_DATA, pConstantBuffer.GetAddressOf(), nullptr, D3D11_USAGE_DYNAMIC);	//Constant Buffer
	indicesCount = i.size();
}

void Mesh::UpdatePosition(const DirectX::XMMATRIX& worldMatrix)
{
	meshData.pos = worldMatrix;
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

	CreateBuffers(v, ind);
}

template<typename T>
void Mesh::CreateBuffer(const int bindFlag, const UINT byteWidth, ID3D11Buffer** bufferPtr, const T& resourceData, const D3D11_USAGE bufferUsage)
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
