#include "Mesh.h"

Mesh::Mesh(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceCon,
	std::vector<Vertex>& vertices, std::vector<DWORD> indices)
{
	pDeviceContext = pDeviceCon;
	indexSize = indices.size();
	CreateBuffer(D3D11_BIND_VERTEX_BUFFER, sizeof Vertex * vertices.size(), pVertexBuffer.GetAddressOf(), vertices);		//Vertex Buffer
	CreateBuffer(D3D11_BIND_INDEX_BUFFER, sizeof DWORD * indices.size(), pIndexBuffer.GetAddressOf(), indices);		//Index Buffer
}

Mesh::Mesh(const Mesh& m)
{
	pDeviceContext = m.pDeviceContext;
	pIndexBuffer = m.pIndexBuffer;
	pVertexBuffer = m.pVertexBuffer;
}

void Mesh::Draw()
{
	//Set Vertex of Obj
	UINT stride = sizeof Vertex;
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
	
	//Set Index Buffer of OBJ
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, NULL);

	//Draw indices of OBJ
	pDeviceContext->DrawIndexed(indexSize, 0, 0);		//Indices to draw | Starting position of indices | Starting position of vertex


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
		hr = pObjDevice->CreateBuffer(&bufferDesc, &bufferData, bufferPtr);
	}
	else
		hr = pObjDevice->CreateBuffer(&bufferDesc, NULL, bufferPtr);

	if (FAILED(hr))
		ErrorMes::DisplayErrMessage(hr);
}
