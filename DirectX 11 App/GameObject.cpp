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

void GameObject::SetTransformations(const DirectX::XMMATRIX& newMatrix)
{
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(newMatrix);
}

void GameObject::SetTransformations(const DirectX::XMFLOAT4X4A& newF4X4)
{
	///Converts XMFLOAT4X4A to XMMATRIX
	const DirectX::XMMATRIX matrix = DirectX::XMLoadFloat4x4A(&newF4X4);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(matrix);
}

const DirectX::XMMATRIX& GameObject::GetTransformationsMatrix()
{
	return object.GetWorldPosition();
}

const DirectX::XMFLOAT4X4A GameObject::GetTransformations4X4()
{
	///Convert matrix to XMFLOAT4X4A
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());
	return float4x4;
}

void GameObject::SetPosition(const DirectX::XMVECTOR& newPosition)
{
	///Converts XMVECTOR to XMMATRIX
	const DirectX::XMMATRIX positionMat = DirectX::XMMatrixTranslationFromVector(newPosition);
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

void GameObject::SetViewProjectionMatrix(const DirectX::XMMATRIX& viewProjMat)
{
	object.SetViewProjectionMatrix(viewProjMat);
}

const DirectX::XMFLOAT3A GameObject::GetPosition3A()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	//TODO: find another way that doesnt involve individual obtaining
	DirectX::XMFLOAT3A float3 = DirectX::XMFLOAT3A(float4x4._41, float4x4._42, float4x4._43);
	return float3;
}

const float GameObject::GetPositionX()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Obtain individual X value from XMFLOAT4X4 variable
	return float4x4._41;
}

const float GameObject::GetPositionY()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Obtain individual Y value from XMFLOAT4X4 variable
	return float4x4._42;
}

const float GameObject::GetPositionZ()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Obtain individual Z value from XMFLOAT4X4 variable
	return float4x4._43;
}

void GameObject::SetRotation(const DirectX::XMVECTOR& newRotation)		///Rotation angles need to be in Radians
{
	///Converts XMVECTOR to XMMATRIX
	const DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYawFromVector(newRotation);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(rotationMat);
}

void GameObject::SetRotation(const DirectX::XMFLOAT3A& newRotation)
{
	DirectX::XMFLOAT3A radianAngles;
	radianAngles.x = DirectX::XMConvertToRadians(newRotation.x);
	radianAngles.y = DirectX::XMConvertToRadians(newRotation.y);
	radianAngles.z = DirectX::XMConvertToRadians(newRotation.z);

	///Converts XMFLOAT3A to XMMATRIX
	const DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(radianAngles.x, radianAngles.y, radianAngles.z);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(rotationMat);
}

void GameObject::SetRotation(const float x, const float y, const float z)
{
	///Convert Angle Degree Parameters into radians
	const float radX = DirectX::XMConvertToRadians(x);
	const float radY = DirectX::XMConvertToRadians(y);
	const float radZ = DirectX::XMConvertToRadians(z);

	//TODO: Look into using either right hand or left hand coordinate system, for right hand, all angles will need to be negated
	//TODO: Obtain matrix from mesh and apply to object.WorldMatrix in case model comes with rotations

	///Converts floats to XMMATRIX
	const DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(radX, radY, radZ);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(rotationMat);
}

const DirectX::XMFLOAT3A GameObject::GetRotation3A()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	DirectX::XMFLOAT3A float3 = DirectX::XMFLOAT3A(float4x4._41, float4x4._42, float4x4._43);
	return float3;
}

const float GameObject::GetRotationX()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	float angle = atan2f(float4x4._32, float4x4._33);
	angle = DirectX::XMConvertToDegrees(angle);

	///Obtain individual X value from XMFLOAT4X4 variable
	return angle;
}

const float GameObject::GetRotationY()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	float angle = atan2f(-float4x4._31, sqrt(pow(float4x4._32, 2) + pow(float4x4._33, 2)));
	angle = DirectX::XMConvertToDegrees(angle);

	///Obtain individual Y value from XMFLOAT4X4 variable
	return angle;
}

const float GameObject::GetRotationZ()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	float angle = atan2f(float4x4._21, float4x4._11);
	angle = DirectX::XMConvertToDegrees(angle);

	///Obtain individual Z value from XMFLOAT4X4 variable
	return angle;
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
