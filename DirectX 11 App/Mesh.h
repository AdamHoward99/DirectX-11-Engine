#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "Vertex.h"
#include "ErrorMessaging.h"
#include "ConstantBufferData.h"

class Mesh
{
public:
	///Constructor used for imported OBJ data from a 3D model file
	Mesh(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceCon, 
		const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices);

	///Constructor used for default triangle rendered ONLY when no filename was given
	Mesh(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceCon);
	
	void UpdatePosition(const DirectX::XMMATRIX& worldMatrix);
	void Draw();

	///Overloaded function for CreateBuffer creating Index, Constant and Vertex buffers
	void CreateBuffers(Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices);

	///Reusable function to create Index, Constant and Vertex buffers
	template<typename T>
	void CreateBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, const int bindFlag, const UINT dataSize, ID3D11Buffer** bufferPtr, const T& resourceData,
		const D3D11_USAGE bufferUsage = D3D11_USAGE_DEFAULT);

private:
	void CreateTriangleGeometry(Microsoft::WRL::ComPtr<ID3D11Device>& device);
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;

	///Stores amount of indices used by Mesh for DrawIndexed function, requires less than storing vector of indices
	UINT indicesCount = 0;
	VS_CB_DATA meshData;
};