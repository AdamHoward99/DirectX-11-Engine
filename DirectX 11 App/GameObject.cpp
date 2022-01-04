#include "GameObject.h"

GameObject::GameObject(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, const std::string& filepath,
	const std::string& gameObjectName)
	:object(pDevice, pDeviceContext, filepath), gameObjectName(gameObjectName)
{}

GameObject::~GameObject()
{}

GameObject& GameObject::operator=(const GameObject& otherObj)
{
	///Ensure this is not a self-assignment
	if (this != &otherObj)
	{
		this->gameObjectName = otherObj.gameObjectName;
		this->object = otherObj.object;
	}

	return *this;
}

GameObject& GameObject::operator=(const GameObject&& otherObj)
{
	if (this != &otherObj)
	{
		///Note: Free any resources of this object
		this->gameObjectName = otherObj.gameObjectName;
		this->object = otherObj.object;

		///Note: Free any resources from other object
	}

	return *this;
}

void GameObject::Update()
{
	object.Update();
}

void GameObject::SetPosition(const DirectX::XMMATRIX& newPosition)
{
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(newPosition);
}

void GameObject::SetPosition(const DirectX::XMVECTOR& newPosition)
{
	///Converts XMVECTOR to XMMATRIX
	const DirectX::XMMATRIX positionMat = DirectX::XMMatrixTranslationFromVector(newPosition);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(positionMat);
}

void GameObject::SetPosition(const DirectX::XMFLOAT4X4A& newPosition)
{
	///Converts XMFLOAT4X4A to XMMATRIX
	const DirectX::XMMATRIX positionMat = DirectX::XMLoadFloat4x4A(&newPosition);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(positionMat);
}

void GameObject::SetPosition(const DirectX::XMFLOAT3A& newPosition)
{
	///Converts XMFLOAT3A to XMMATRIX
	const DirectX::XMMATRIX positionMat = DirectX::XMMatrixTranslation(newPosition.x, newPosition.y, newPosition.z);		//TODO: Test if conversion to XMVECTOR is more efficient than singly obtaining the values
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(positionMat);
}

void GameObject::SetPosition(const float x, const float y, const float z)
{
	///Converts floats to XMMATRIX
	const DirectX::XMMATRIX positionMat = DirectX::XMMatrixTranslation(x, y, z);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(positionMat);
}

void GameObject::SetPositionInView(const DirectX::XMMATRIX& newPosition)
{
	///Passes value to Object class to change Mesh position based on Camera
	object.SetWorldPositionInViewport(newPosition);
}

const DirectX::XMMATRIX& GameObject::GetPositionMatrix()
{
	return object.GetWorldPosition();
}

const DirectX::XMFLOAT4X4A GameObject::GetPosition4X4()
{
	///Convert matrix to XMFLOAT4X4A
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());
	return float4x4;
}

const DirectX::XMFLOAT3A GameObject::GetPosition3A()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	//TODO: find another way that doesnt involve individual obtaining
	DirectX::XMFLOAT3A float3 = DirectX::XMFLOAT3A(float4x4._14, float4x4._24, float4x4._34);
	return float3;
}

const float GameObject::GetPositionX()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Obtain individual X value from XMFLOAT4X4 variable
	return float4x4._14;
}

const float GameObject::GetPositionY()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Obtain individual Y value from XMFLOAT4X4 variable
	return float4x4._24;
}

const float GameObject::GetPositionZ()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Obtain individual Z value from XMFLOAT4X4 variable
	return float4x4._34;
}

void* GameObject::operator new(size_t i)
{
	///Aligns to 16-bits for unique_ptr creation
	return _mm_malloc(i, 16);
}

void GameObject::operator delete(void* p)
{
	_mm_free(p);
}
