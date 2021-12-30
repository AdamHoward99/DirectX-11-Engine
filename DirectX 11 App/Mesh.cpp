#include "Mesh.h"

Mesh::Mesh(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceCon,
	const std::vector<Vertex>& v, const std::vector<DWORD>& i, const std::vector<Texture>& textures)
{
	pDeviceContext = deviceCon;
	meshTextures = textures;
	CreateBuffers(device, v, i);
}

Mesh::Mesh(const Mesh& m)
{
	pVertexBuffer = m.pVertexBuffer;
	pIndexBuffer = m.pIndexBuffer;
	pConstantBuffer = m.pConstantBuffer;
	pDeviceContext = m.pDeviceContext;
	
	indicesCount = m.indicesCount;
	meshData = m.meshData;
	meshTextures = m.meshTextures;
}

Mesh::Mesh(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	pDeviceContext = deviceContext;
	CreateTriangleGeometry(device);
}

void Mesh::Draw()
{
	///Sets Vertices for OBJ using Vertex buffer
	UINT stride = sizeof Vertex;
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
	///IASetVertexBuffers(IN, IN, OPTIONAL, OPTIONAL, OPTIONAL)
	///UINT StartSlot                 - Starting position of array for vertex resources
	///UINT NumBuffers                - The number of vertex buffers in use
	///ID3D11Buffer** ppVertexBuffers - Pointer to an array of the vertex buffers in use
	///const UINT* pStrides			  - Pointer to an array of stride values which are the byte size of elements used by the buffer
	///const UINT* pOffsets			  - Pointer to an array of offset values which are the offset between elements in buffer
	
	///Draw Texture
	for (Texture& t : meshTextures)
		pDeviceContext->PSSetShaderResources(0, 1, t.GetTextureRV().GetAddressOf());
	///PSSetShaderResources(IN, IN, OPTIONAL)
	///UINT StartSlot			 - Index of array to begin setting resources
	///UINT NumViews			 - Amount of shader resources required to be set up, maximum of 128
	///ID3D11ShaderResourceView* - Array of SRV interfaces to set to device

	///Set Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = pDeviceContext->Map(pConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
	///Map(IN, IN, IN, OPTIONAL, OPTIONAL)
	///ID3D11Resource* pResource - Pointer to Constant Buffer
	///UINT Subresource			 - Subresource index number
	///D3D11_MAP MapType		 - Specifies CPU's read and write permissions for constant buffer
	///UINT MapFlags			 - Flag that specifies what CPU does when GPU is busy
	///D3D11_MAPPED_SUBRESOURCE* - Pointer to mapped struct for constant buffer resource
	
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11DeviceContext::Map()");

	CopyMemory(mapRes.pData, &meshData, sizeof VS_CB_DATA);
	pDeviceContext->Unmap(pConstantBuffer.Get(), NULL);
	///Unmap(IN, IN)
	///ID3D11Resource* pResource - Pointer to the constant buffer
	///UINT Subresource			 - index of a subresource which needs to be unmapped

	pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	///VSSetConstantBuffers(IN, IN, OPTIONAL)
	///UINT StartSlot					- Starting index of array for constant buffer
	///UINT NumBuffers					- Number of constant buffers to set
	///ID3D11Buffer** ppConstantBuffers - Pointer to array of constant buffers to set

	///Set Indices of OBJ using Index Buffer
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, NULL);
	///IASetIndexBuffer(OPTIONAL, IN, IN)
	///ID3D11Buffer* pIndexBuffer - Pointer to Index buffer object (Contains Indices of OBJ) 
	///DXGI_FORMAT Format		  - Specifies format of data in Index buffer (Either DXGI_FORMAT_R16_UINT | DXGI_FORMAT_R32_UINT)
	///UINT Offset				  - Byte offset from start of Index buffer to first use

	///Draw indices of OBJ on screen
	pDeviceContext->DrawIndexed(indicesCount, 0, 0);
	///DrawIndexed(IN, IN, IN)
	///UINT IndexCount - Number of indices to draw
	///UINT StartIndexLocation - Starting position of indices
	///INT BaseVertexLocation - Value added to every index before reading from VB
}

void Mesh::CreateBuffers(Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::vector<Vertex>& v, const std::vector<DWORD>& i)
{
	///Vertex Buffer
	CreateBuffer(device, D3D11_BIND_VERTEX_BUFFER, sizeof Vertex * v.size(), pVertexBuffer.GetAddressOf(), v.data());
	///Index Buffer
	CreateBuffer(device, D3D11_BIND_INDEX_BUFFER, sizeof DWORD * i.size(), pIndexBuffer.GetAddressOf(), i.data());				
	///Constant Buffer
	CreateBuffer(device, D3D11_BIND_CONSTANT_BUFFER, sizeof VS_CB_DATA, pConstantBuffer.GetAddressOf(), nullptr, D3D11_USAGE_DYNAMIC);	
	///Store Indices count for DrawIndexed()
	indicesCount = i.size();
}

Mesh::~Mesh()
{}

Mesh& Mesh::operator=(const Mesh& otherMesh)
{
	///Ensure this is not a self-assignment
	if (this != &otherMesh)
	{
		this->pVertexBuffer = otherMesh.pVertexBuffer;
		this->pIndexBuffer = otherMesh.pIndexBuffer;
		this->pConstantBuffer = otherMesh.pConstantBuffer;
		this->pDeviceContext = otherMesh.pDeviceContext;

		this->indicesCount = otherMesh.indicesCount;
		this->meshData = otherMesh.meshData;
		this->meshTextures = otherMesh.meshTextures;
	}

	return *this;
}

void Mesh::UpdatePosition(const DirectX::XMMATRIX& worldMatrix)
{
	///Updates world position of Mesh/Object each Update() call
	meshData.pos = worldMatrix;
}

void Mesh::CreateTriangleGeometry(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	///Manually creating Vertices and Indices for default triangle model
	std::vector<Vertex> v =
	{
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

	CreateBuffers(device, v, ind);
}

template<typename T>
void Mesh::CreateBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, const int bindFlag, const UINT byteWidth, ID3D11Buffer** bufferPtr, const T& resourceData, const D3D11_USAGE bufferUsage)
{
	///Create Buffer Desc
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof D3D11_BUFFER_DESC);

	bufferDesc.BindFlags = bindFlag;
	bufferDesc.ByteWidth = byteWidth;
	///Sets to 0 for Index and Vertex Buffers, Gives Write Access for Constant Buffer Only
	bufferDesc.CPUAccessFlags = (bufferUsage == D3D11_USAGE_DEFAULT) ? 0 : D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = bufferUsage;

	HRESULT hr;

	///If data has been passed to this function (Index and Vertex Buffers)
	if (resourceData != nullptr)
	{
		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof D3D11_SUBRESOURCE_DATA);
		bufferData.pSysMem = resourceData;
		hr = device->CreateBuffer(&bufferDesc, &bufferData, bufferPtr);
	}
	else
		hr = device->CreateBuffer(&bufferDesc, NULL, bufferPtr);		///Constant Buffer Creation

	///CreateBuffer(IN, OPTIONAL, OPTIONAL)
	///const D3D11_BUFFER_DESC* pDesc - Pointer to Buffer description for buffer
	///const D3D11_SUBRESOURCE_DATA* pInitialData - Pointer to subresource data struct describing init data
	///ID3D11Buffer** ppBuffer - Address of a pointer to the buffer object

	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, __LINE__, __FILE__, "ID3D11Device::CreateBuffer()");
}
