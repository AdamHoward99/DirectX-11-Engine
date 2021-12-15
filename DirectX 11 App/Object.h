#pragma once
#include <d3d11.h>
#include <string>
#include <wrl/client.h>
#include <WICTextureLoader.h>
#include "Vertex.h"
#include "ConstantBufferData.h"
#include "ErrorMessaging.h"

class Object			//Model
{
public:
	Object(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext);
	~Object();

	void* operator new(size_t i);
	void operator delete(void* p);

	void Initialize();
	void Update();
	void Render();
	void LoadTexture(const std::wstring& texturePath);

	void CreateObjBuffers();		//Overloaded function for CreateBuffer function

	template<typename T>
	void CreateBuffer(const int bindFlag, const UINT dataSize, ID3D11Buffer** bufferPtr, const T& resourceData,
		const D3D11_USAGE bufferUsage = D3D11_USAGE_DEFAULT);		//Reusable function for creating index, vertex and constant buffers

	void SetWorldPosition(const DirectX::XMMATRIX& pos);
	const DirectX::XMMATRIX& GetWorldPosition() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pObjDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pObjDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pObjTexture;

	DirectX::XMMATRIX objWorldMatrix;
	VS_CB_DATA objData;

	/*TODO: in future, GameObject class will inherit Object class adding:
	position variables
	rotation variables
	scaling variables
	collision
	etc.
	*/
};