#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Vertex.h"

class Mesh
{
public:
	Mesh(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceCon, 
		std::vector<Vertex>& vertices, std::vector<DWORD> indices);
	
	Mesh(const Mesh& m);
	void Draw();

	template<typename T>
	void CreateBuffer(const int bindFlag, const UINT dataSize, ID3D11Buffer** bufferPtr, const T& resourceData,
		const D3D11_USAGE bufferUsage = D3D11_USAGE_DEFAULT);		//Reusable function for creating index, vertex and constant buffers

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;

	UINT indexSize = 0;
};