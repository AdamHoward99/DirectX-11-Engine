#include "GameObject.h"

GameObject::GameObject(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, const std::string& filepath,
	const std::string& gameObjectName)
	:object(pDevice, pDeviceContext, filepath), gameObjectName(gameObjectName)
{}

GameObject::GameObject(const GameObject& otherObj)
	: object(otherObj.object), gameObjectName(otherObj.gameObjectName + "_Copy")
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

void GameObject::SetViewProjectionMatrix(const DirectX::XMMATRIX& viewProjMat)
{
	object.SetViewProjectionMatrix(viewProjMat);
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

///Position Functions----------------------------------------------------------------------
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
	const DirectX::XMMATRIX positionMat = DirectX::XMMatrixTranslation(newPosition.x, newPosition.y, newPosition.z);
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

const DirectX::XMVECTOR GameObject::GetPositionVec()
{
	///Get XMFLOAT3A from XMMATRIX
	const DirectX::XMFLOAT3A float3 = GetPosition3A();
	///Convert XMFLOAT3A to XMVECTOR
	DirectX::XMVECTOR vector = DirectX::XMLoadFloat3A(&float3);
	return vector;
}

const DirectX::XMFLOAT3A GameObject::GetPosition3A()
{
	///Store matrix into XMFLOAT4X4A variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

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
///End of Position Functions----------------------------------------------------------------------

///Rotation Functions----------------------------------------------------------------------
void GameObject::SetRotation(const DirectX::XMVECTOR& newRotation)		///Rotation angles need to be in Radians
{
	///Converts XMVECTOR to XMMATRIX
	const DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYawFromVector(newRotation);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(rotationMat);
}

void GameObject::SetRotation(const DirectX::XMFLOAT3A& newRotation)		///Rotation angles need to be in Degrees
{
	///Convert Degree angles to Radians
	DirectX::XMFLOAT3A radianAngles;
	radianAngles.x = DirectX::XMConvertToRadians(newRotation.x);
	radianAngles.y = DirectX::XMConvertToRadians(newRotation.y);
	radianAngles.z = DirectX::XMConvertToRadians(newRotation.z);

	///Converts XMFLOAT3A to XMMATRIX
	const DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(radianAngles.x, radianAngles.y, radianAngles.z);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(rotationMat);
}

void GameObject::SetRotation(const float x, const float y, const float z)	///Rotation angles need to be in Degrees
{
	///Convert Angle Degree Parameters into radians
	const float radX = DirectX::XMConvertToRadians(x);
	const float radY = DirectX::XMConvertToRadians(y);
	const float radZ = DirectX::XMConvertToRadians(z);

	///Converts floats to XMMATRIX
	const DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(radX, radY, radZ);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(rotationMat);
}

const DirectX::XMVECTOR GameObject::GetRotationVec()
{
	///Get XMFLOAT3A from XMMATRIX
	const DirectX::XMFLOAT3A float3 = GetRotation3A();
	///Convert XMFLOAT3A to XMVECTOR
	DirectX::XMVECTOR vector = DirectX::XMLoadFloat3A(&float3);
	return vector;
}

const DirectX::XMFLOAT3A GameObject::GetRotation3A()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	DirectX::XMFLOAT3A angles;
	angles.x = DirectX::XMConvertToDegrees(atan2f(float4x4._32, float4x4._33));
	angles.y = DirectX::XMConvertToDegrees(atan2f(float4x4._21, float4x4._11));
	angles.z = DirectX::XMConvertToDegrees(atan2f(-float4x4._31, sqrt(pow(float4x4._32, 2) + pow(float4x4._33, 2))));
	return angles;
}

const float GameObject::GetRotationX()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Calculates X rotation angle in Degrees using formula: atan2(r32, r33)
	float angle = atan2f(float4x4._32, float4x4._33);
	return DirectX::XMConvertToDegrees(angle);
}

const float GameObject::GetRotationY()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Calculates Y rotation angle in Degrees using formula: atan2(r21, r11)
	float angle = atan2f(float4x4._21, float4x4._11);
	return DirectX::XMConvertToDegrees(angle);
}

const float GameObject::GetRotationZ()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Calculates Z rotation angle in Degrees using formula: atan2(-r31, sqrt(r32 ^ 2 + r33 ^ 2)
	float angle = atan2f(-float4x4._31, sqrt(pow(float4x4._32, 2) + pow(float4x4._33, 2)));
	return DirectX::XMConvertToDegrees(angle);
}
///End of Rotation Functions----------------------------------------------------------------------

///Scale Functions----------------------------------------------------------------------
void GameObject::SetScale(const DirectX::XMVECTOR& newScale)
{
	///Converts XMVECTOR to XMMATRIX
	const DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(newScale);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(scaleMat);
}

void GameObject::SetScale(const DirectX::XMFLOAT3A& newScale)
{
	///Converts XMFLOAT3A to XMMATRIX
	const DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(newScale.x, newScale.y, newScale.z);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(scaleMat);
}

void GameObject::SetScale(const float x, const float y, const float z)
{
	///Convert floats to XMMATRIX
	const DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(x, y, z);
	///Passes value to Object class to change Mesh position
	object.SetWorldPosition(scaleMat);
}

const DirectX::XMVECTOR GameObject::GetScaleVec()
{
	///Get XMFLOAT3A version of XMMATRIX
	const DirectX::XMFLOAT3A float3 = GetScale3A();
	///Convert XMFLOAT3A to XMVECTOR
	DirectX::XMVECTOR vector = DirectX::XMLoadFloat3A(&float3);
	return vector;
}

const DirectX::XMFLOAT3A GameObject::GetScale3A()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());
	DirectX::XMFLOAT3A scaleValues;

	///Work out X Scale Value from Transformation Matrix
	scaleValues.x = sqrtf(powf(float4x4._11, 2) + powf(float4x4._12, 2) + powf(float4x4._13, 2));
	///Work out Y Scale Value from Transformation Matrix
	scaleValues.y = sqrtf(powf(float4x4._31, 2) + powf(float4x4._32, 2) + powf(float4x4._33, 2));
	///Work out Z Scale Value from Transformation Matrix
	scaleValues.z = sqrtf(powf(float4x4._21, 2) + powf(float4x4._22, 2) + powf(float4x4._23, 2));
	return scaleValues;
}

const float GameObject::GetScaleX()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Work out X Scale Value from Transformation Matrix
	float scaleX = sqrtf(powf(float4x4._11, 2) + powf(float4x4._12, 2) + powf(float4x4._13, 2));
	return scaleX;
}

const float GameObject::GetScaleY()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Work out Y Scale Value from Transformation Matrix
	float scaleY = sqrtf(powf(float4x4._31, 2) + powf(float4x4._32, 2) + powf(float4x4._33, 2));
	return scaleY;
}

const float GameObject::GetScaleZ()
{
	///Store matrix into XMFLOAT4X4 variable
	DirectX::XMFLOAT4X4A float4x4;
	DirectX::XMStoreFloat4x4A(&float4x4, object.GetWorldPosition());

	///Work out Z Scale Value from Transformation Matrix
	float scaleZ = sqrtf(powf(float4x4._21, 2) + powf(float4x4._22, 2) + powf(float4x4._23, 2));
	return scaleZ;
}
///End of Scale Functions----------------------------------------------------------------------

///Collision Functions----------------------------------------------------------------------
bool GameObject::CheckCollisionAABB(const DirectX::XMFLOAT3A& otherObject)
{
	//TODO: Test with multiple different models
	///Get Position of this GameObject
	const DirectX::XMFLOAT3A thisPosition = GetPosition3A();

	///Detect if GameObjects are colliding using Axis-Aligned Bounding Box method
	if (fabsf(thisPosition.x - otherObject.x) < 5.f)
		if (fabsf(thisPosition.y - otherObject.y) < 5.f)
			if (fabsf(thisPosition.z - otherObject.z) < 5.f)
				return true;

	return false;
}

bool GameObject::CheckCollisionAABB(const DirectX::XMFLOAT3A & otherObject, const float distance)
{
	///Get Position of this GameObject
	const DirectX::XMFLOAT3A thisPosition = GetPosition3A();

	///Detect if GameObjects are colliding using Axis-Aligned Bounding Box method
	if (fabsf(thisPosition.x - otherObject.x) < distance)
		if (fabsf(thisPosition.y - otherObject.y) < distance)
			if (fabsf(thisPosition.z - otherObject.z) < distance)
				return true;

	return false;
}
///End of Collision Functions----------------------------------------------------------------------

///LookAt Functions----------------------------------------------------------------------
void GameObject::LookAt(const DirectX::XMFLOAT3A& differencePos)
{
	//TODO: Test with a scene containing more than 1 model, preferably with a model which has a face
	///Calculate distance between using Pythagoras' theorem
	const float distance = sqrtf(powf(differencePos.x, 2.f) + powf(differencePos.z, 2.f));

	///Calculate Pitch using formula: (Y / sqrt(X ^ 2 + Z ^ 2))
	const float pitchAngle = atanf(differencePos.y * (1.f / distance));

	///Calculate Yaw using formula: (X / Z) + PI
	float yawAngle = atanf(differencePos.x * (1.f / differencePos.z));
	///Yaw angle needs to be larger than radian range atanf allows
	if (differencePos.z > 0.f)
		yawAngle += DirectX::XM_PI;

	///Apply Pitch and Yaw angles to GameObjects rotation, Has to be XMVECTOR since angles are in Radians
	SetRotation(DirectX::XMVectorSet(pitchAngle, yawAngle, 0.f, 0.f));
}

void GameObject::SetLookAt(const DirectX::XMMATRIX& targetPos)
{
	///Get the position of this GameObject
	const DirectX::XMFLOAT4X4A thisPosition = GetTransformations4X4();

	///Convert Target position XMMATRIX into XMFLOAT4X4A
	DirectX::XMFLOAT4X4A targetPos4x4;
	DirectX::XMStoreFloat4x4A(&targetPos4x4, targetPos);

	///Get the difference of position between this and target
	DirectX::XMFLOAT3A difference;
	difference.x = thisPosition._41 - targetPos4x4._41;
	difference.y = thisPosition._42 - targetPos4x4._42;
	difference.z = thisPosition._43 - targetPos4x4._43;

	LookAt(difference);
}

void GameObject::SetLookAt(const DirectX::XMVECTOR& targetPos)
{
	///Get the position of this GameObject
	const DirectX::XMFLOAT3A thisPosition = GetPosition3A();

	///Convert Target position XMVECTOR into XMFLOAT3A
	DirectX::XMFLOAT3A targetPos3A;
	DirectX::XMStoreFloat3A(&targetPos3A, targetPos);

	///Get the difference of position between this and target
	DirectX::XMFLOAT3A difference;
	difference.x = thisPosition.x - targetPos3A.x;
	difference.y = thisPosition.y - targetPos3A.y;
	difference.z = thisPosition.z - targetPos3A.z;

	LookAt(difference);
}

void GameObject::SetLookAt(const DirectX::XMFLOAT3A& targetPos)
{
	///Get the position of this GameObject
	const DirectX::XMFLOAT3A thisPosition = GetPosition3A();

	///Get the difference of position between this and target
	DirectX::XMFLOAT3A difference;
	difference.x = thisPosition.x - targetPos.x;
	difference.y = thisPosition.y - targetPos.y;
	difference.z = thisPosition.z - targetPos.z;

	LookAt(difference);
}

void GameObject::SetLookAt(const float x, const float y, const float z)
{
	///Get the position of this GameObject
	const DirectX::XMFLOAT3A thisPosition = GetPosition3A();

	///Get the difference of position between this and target
	DirectX::XMFLOAT3A difference;
	difference.x = thisPosition.x - x;
	difference.y = thisPosition.y - y;
	difference.z = thisPosition.z - z;

	LookAt(difference);
}
///End of LookAt Functions----------------------------------------------------------------------

void* GameObject::operator new(size_t i)
{
	///Aligns to 16-bits for unique_ptr creation
	return _mm_malloc(i, 16);
}

void GameObject::operator delete(void* p)
{
	_mm_free(p);
}
