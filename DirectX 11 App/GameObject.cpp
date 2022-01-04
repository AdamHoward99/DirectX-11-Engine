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

void* GameObject::operator new(size_t i)
{
	///Aligns to 16-bits for unique_ptr creation
	return _mm_malloc(i, 16);
}

void GameObject::operator delete(void* p)
{
	_mm_free(p);
}
