#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <WICTextureLoader.h>
#include <vector>
#include "Vertex.h"
#include "ErrorMessaging.h"
#include "ConstantBufferData.h"

class Mesh
{
public:
	Mesh(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceCon, 
		const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices);

	//Constructor to create Default geometry
	Mesh(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceCon);
	
	//Mesh(const Mesh& m);
	void Draw();

	void CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices);		//Reusable function for creating index, vertex and constant buffers

	template<typename T>
	void CreateBuffer(const int bindFlag, const UINT dataSize, ID3D11Buffer** bufferPtr, const T& resourceData,
		const D3D11_USAGE bufferUsage = D3D11_USAGE_DEFAULT);		//Reusable function for creating index, vertex and constant buffers

	void UpdatePosition(const DirectX::XMMATRIX& worldMatrix);
	void LoadTexture(const std::wstring& texturePath);

private:
	void CreateTriangleGeometry();
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;

	UINT indicesCount = 0;
	VS_CB_DATA meshData;
};