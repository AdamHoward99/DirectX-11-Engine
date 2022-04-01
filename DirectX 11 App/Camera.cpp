#include "Camera.h"

///Count of total cameras used in the scene
int Camera::CamerasInUse = 0;

Camera::Camera()
{
	///Set Default Values
	cameraPosition = DirectX::XMFLOAT3A(0.0f, 0.0f, 0.0f);
	cameraRotation = DirectX::XMFLOAT3A(0.f, 0.f, 0.f);

	cameraPositionVec = DirectX::XMVectorReplicate(0.f);
	cameraRotationVec = DirectX::XMVectorReplicate(0.f);

	CameraNo = CamerasInUse;
	CamerasInUse++;

	UpdateView();
}

Camera::Camera(const DirectX::XMFLOAT3A& camPos, const DirectX::XMFLOAT3A& camRot)
{
	cameraPosition = camPos;
	cameraRotation = camRot;

	CameraNo = CamerasInUse;
	CamerasInUse++;

	UpdateView();
}

Camera::Camera(const DirectX::XMVECTOR& camPos, const DirectX::XMVECTOR& camRot)
{
	cameraPositionVec = camPos;
	cameraRotationVec = camRot;

	CameraNo = CamerasInUse;
	CamerasInUse++;

	UpdateView();
}

Camera::~Camera()
{}

void Camera::SetupCamera(const DirectX::XMFLOAT3A& startPos, const float aspRatio, const float nearZ, const float farZ)
{
	SetPosition(startPos);

	aspectRatio = aspRatio;
	this->nearZ = nearZ;
	this->farZ = farZ;

	SetProjection(90.f, aspectRatio, this->nearZ, this->farZ);
}

///+= Overload for XMFLOAT3A
const DirectX::XMFLOAT3A& operator+=(DirectX::XMFLOAT3A& A, const DirectX::XMFLOAT3A& B)
{
	A.x = A.x + B.x;
	A.y = A.y + B.y;
	A.z = A.z + B.z;
	return A;
}

///*= overload for XMFLOAT3A
const DirectX::XMFLOAT3A& operator*=(DirectX::XMFLOAT3A& A, const float B)
{
	A.x = A.x * B;
	A.y = A.y * B;
	A.z = A.z * B;
	return A;
}

void Camera::SetProjection(float degrees, float aspRatio, float nearZ, float farZ)
{
	degrees = DirectX::XMConvertToRadians(degrees);
	projection = DirectX::XMMatrixPerspectiveFovLH(degrees, aspRatio, nearZ, farZ);
}

//XMFLOAT Functions----------------------------------------------------------------------//
void Camera::SetPosition(const DirectX::XMFLOAT3A& newPos)
{
	cameraPosition = newPos;
	UpdateView();
}

void Camera::AddToPosition(const DirectX::XMFLOAT3A& newPos)
{
	cameraPosition += newPos;
	UpdateView();
}

const DirectX::XMFLOAT3A& Camera::GetPosition3A() const
{
	return cameraPosition;
}

void Camera::SetRotation(const DirectX::XMFLOAT3A& newRot)
{
	cameraRotation = newRot;
	UpdateView();
}

void Camera::AddToRotation(const DirectX::XMFLOAT3A& rot)
{
	cameraRotation += rot;
	UpdateView();
}

const DirectX::XMFLOAT3A& Camera::GetRotation3A() const
{
	return cameraRotation;
}
//End of XMFLOAT Functions--------------------------------------------------------------//

//XMVECTOR Functions----------------------------------------------------------------------//
void Camera::SetPosition(const DirectX::XMVECTOR& newPos)
{
	cameraPositionVec = newPos;
	UpdateView();
}

void Camera::AddToPosition(const DirectX::XMVECTOR& newPos)
{
	cameraPositionVec = DirectX::XMVectorAdd(cameraPositionVec, newPos);
	UpdateView();
}

const DirectX::XMVECTOR& Camera::GetPositionVec() const
{
	return cameraPositionVec;
}

void Camera::SetRotation(const DirectX::XMVECTOR& newRot)
{
	cameraRotationVec = newRot;
	UpdateView();
}

void Camera::AddToRotation(const DirectX::XMVECTOR& rot)
{
	cameraRotationVec = DirectX::XMVectorAdd(cameraRotationVec, rot);
	UpdateView();
}

const DirectX::XMVECTOR& Camera::GetRotationVec() const
{
	return cameraRotationVec;
}
//End of XMVECTOR Functions--------------------------------------------------------------//

//Singular Float Functions---------------------------------------------------------------//
void Camera::SetPosition(const float X, const float Y, const float Z)
{
	cameraPosition = DirectX::XMFLOAT3A(X, Y, Z);
	UpdateView();
}

void Camera::AddToPosition(const float X, const float Y, const float Z)
{
	cameraPosition += DirectX::XMFLOAT3A(X, Y, Z);
	UpdateView();
}

const float Camera::GetPositionX() const
{
	return cameraPosition.x;
}

const float Camera::GetPositionY() const
{
	return cameraPosition.y;
}

const float Camera::GetPositionZ() const
{
	return cameraPosition.z;
}

void Camera::SetRotation(const float X, const float Y, const float Z)
{
	cameraRotation = DirectX::XMFLOAT3A(X, Y, Z);
	UpdateView();
}

void Camera::AddToRotation(const float X, const float Y, const float Z)
{
	cameraRotation += DirectX::XMFLOAT3A(X, Y, Z);
	UpdateView();
}

const float Camera::GetRotationX() const
{
	return cameraRotation.x;
}

const float Camera::GetRotationY() const
{
	return cameraRotation.y;
}

const float Camera::GetRotationZ() const
{
	return cameraRotation.z;
}
//End of Singular Float Functions--------------------------------------------------------//

void Camera::SetMouseMovement(bool foo)
{
	mouseMovementEnabled = foo;
}

bool Camera::GetMouseMovement() const
{
	return mouseMovementEnabled;
}

const DirectX::XMMATRIX& Camera::GetCameraView() const
{
	return cameraView;
}

const DirectX::XMMATRIX& Camera::GetProjection() const
{
	return projection;
}

const int Camera::GetCameraNumber() const
{
	return CameraNo;
}

void Camera::UpdateView()
{
	//Calculate camera rotation matrix
	DirectX::XMMATRIX cameraRotMatrix = DirectX::XMMatrixRotationRollPitchYaw(cameraRotation.x, cameraRotation.y, cameraRotation.z);

	//Calculate unit vector
	DirectX::XMVECTOR cameraTarget = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), cameraRotMatrix);

	//Adjust camera target
	DirectX::XMVectorAdd(cameraTarget, DirectX::XMLoadFloat3A(&cameraPosition));

	//Calculate Up direction based on current rotation
	DirectX::XMVECTOR upDirection = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f), cameraRotMatrix);

	//Rebuild view matrix
	cameraView = DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3A(&cameraPosition), cameraTarget, upDirection);

	//Get Rotation for forward & right directions

	///Note: XMFLOAT3A Method
	forwardDir = DirectX::XMFLOAT3A(0.f, cameraRotation.y, 1.f);
	rightDir = DirectX::XMFLOAT3A(1.f, cameraRotation.y, 0.f);
	upDir = DirectX::XMFLOAT3A(0.f, 1.f, 0.f);

	///Note: XMVECTOR Method
	forwardVec = DirectX::XMLoadFloat3A(&DirectX::XMFLOAT3A(0.f, cameraRotation.y, 1.f));
	rightVec = DirectX::XMLoadFloat3A(&DirectX::XMFLOAT3A(1.f, cameraRotation.y, 0.f));
	upVec = DirectX::XMLoadFloat3A(&DirectX::XMFLOAT3A(upDir));
}

void Camera::LookAt(const DirectX::XMMATRIX& targetPos)
{
	//Convert XMMATRIX parameter to XMFLOAT4X4A
	DirectX::XMFLOAT4X4A targetPosFloat; 
	DirectX::XMStoreFloat4x4A(&targetPosFloat, targetPos);
	
	//Get Coordinates
	float x = targetPosFloat._14;
	float y = targetPosFloat._24;
	float z = targetPosFloat._34;

	//Get Difference of position between camera and target
	x = cameraPosition.x - x;
	y = cameraPosition.y - y;
	z = cameraPosition.z - z;

	//Calculate distance using pythagoras' theorem
	const float distance = sqrtf(powf(x, 2.f) + powf(z, 2.f));

	//Apply pitch to camera rotation
	SetPitch(y, distance);

	//Apply yaw to camera rotation
	SetYaw(x, z);
}

void Camera::LookAt(DirectX::XMFLOAT3A& target)
{
	///Calculate the Pitch Angle

	//Get Difference of position between camera and target
	target.x = cameraPosition.x - target.x;
	target.y = cameraPosition.y - target.y;
	target.z = cameraPosition.z - target.z;

	//Calculate distance using pythagoras' theorem
	const float distance = sqrtf(powf(target.x, 2.f) + powf(target.z, 2.f));

	//Apply pitch to camera rotation
	SetPitch(target.y, distance);

	//Apply yaw to camera rotation
	SetYaw(target.x, target.z);
}

void Camera::LookAt(float targetX, float targetY, float targetZ)
{
	//Get Difference of position between camera and target
	targetX = cameraPosition.x - targetX;
	targetY = cameraPosition.y - targetY;
	targetZ = cameraPosition.z - targetZ;

	//Calculate distance using pythagoras' theorem
	const float distance = sqrtf(powf(targetX, 2.f) + powf(targetZ, 2.f));

	//Apply pitch to camera rotation
	SetPitch(targetY, distance);

	//Apply yaw to camera rotation
	SetYaw(targetX, targetZ);
}

void Camera::SetPitch(const float y, const float distance)
{
	///Calculate Pitch (Y / sqrt(X ^ 2 + Z ^ 2))
	float pitch = atanf(y * (1.f / distance));
	cameraRotation.x = pitch;
}

void Camera::SetYaw(const float x, const float z)
{
	///Calculate Yaw (X / Z) + PI
	float yaw = atanf(x * (1.f / z));

	if(z > 0.0f)	//Angle needs to be larger than radian range atanf allows
		yaw += DirectX::XM_PI;

	cameraRotation.y = yaw;
}

//Debug Camera Movement Functions---------------------------------------------------------------//

void Camera::MoveCameraForward3A_D(const float dt)
{
	forwardDir *= (dt * cameraSpeed);
	AddToPosition(forwardDir);
}

void Camera::MoveCameraForwardVec_D(const float dt)
{
	forwardVec = DirectX::XMVectorMultiply(forwardVec, DirectX::XMVectorReplicate(dt * cameraSpeed));
	AddToPosition(forwardVec);
}

void Camera::MoveCameraBackwards3A_D(const float dt)
{
	//Reverse / Negative of the forward direction
	forwardDir *= (-dt * cameraSpeed);
	AddToPosition(forwardDir);
}

void Camera::MoveCameraBackwardsVec_D(const float dt)
{
	forwardVec = DirectX::XMVectorMultiply(forwardVec, DirectX::XMVectorReplicate(-dt * cameraSpeed));
	AddToPosition(forwardVec);
}

void Camera::MoveCameraLeft3A_D(const float dt)
{
	//Reverse / Negative of the right direction
	rightDir *= (-dt * cameraSpeed);
	AddToPosition(rightDir);
}

void Camera::MoveCameraLeftVec_D(const float dt)
{
	rightVec = DirectX::XMVectorMultiply(rightVec, DirectX::XMVectorReplicate(-dt * cameraSpeed));
	AddToPosition(rightVec);
}

void Camera::MoveCameraRight3A_D(const float dt)
{
	rightDir *= (dt * cameraSpeed);
	AddToPosition(rightDir);
}

void Camera::MoveCameraRightVec_D(const float dt)
{
	rightVec = DirectX::XMVectorMultiply(rightVec, DirectX::XMVectorReplicate(dt * cameraSpeed));
	AddToPosition(rightVec);
}

void Camera::ElevateCamera3A_D(const float dt)
{
	upDir *= (dt * cameraSpeed);
	AddToPosition(upDir);
}

void Camera::ElevateCameraVec_D(const float dt)
{
	upVec = DirectX::XMVectorMultiply(upVec, DirectX::XMVectorReplicate(dt * cameraSpeed));
	AddToPosition(upVec);
}

void Camera::LowerCamera3A_D(const float dt)
{
	//Reverse / Negative of the Up direction
	upDir *= (-dt * cameraSpeed);
	AddToPosition(upDir);
}

void Camera::LowerCameraVec_D(const float dt)
{
	upVec = DirectX::XMVectorMultiply(upVec, DirectX::XMVectorReplicate(-dt * cameraSpeed));
	AddToPosition(upVec);
}
//End of Debug Camera Movement Functions--------------------------------------------------------//
