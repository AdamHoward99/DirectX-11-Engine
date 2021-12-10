#include "Camera.h"

int Camera::CamerasInUse = 0;		//Counts total cameras used across all scenes

Camera::Camera()
{
	//Set Default Values
	cameraPosition = DirectX::XMFLOAT3A(0.0f, 0.0f, 0.0f);
	cameraRotation = DirectX::XMFLOAT3A(0.f, 0.f, 0.f);

	cameraPositionVec = DirectX::XMVectorReplicate(0.f);
	cameraRotationVec = DirectX::XMVectorReplicate(0.f);

	CameraNo = CamerasInUse;
	CamerasInUse++;

	UpdateView();
}

void Camera::SetupCamera(const DirectX::XMFLOAT3A& startPos, const float aspRatio, const float nearZ, const float farZ)
{
	SetPosition(startPos);

	aspectRatio = aspRatio;
	this->nearZ = nearZ;
	this->farZ = farZ;

	SetProjection(90.f, aspectRatio, this->nearZ, this->farZ);
}

//+= Overload for XMFLOAT3A
const DirectX::XMFLOAT3A& operator+=(DirectX::XMFLOAT3A& A, const DirectX::XMFLOAT3A& B)
{
	A.x = A.x + B.x;
	A.y = A.y + B.y;
	A.z = A.z + B.z;
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

void Camera::MovePosition(const DirectX::XMFLOAT3A& newPos)
{
	cameraPosition += newPos;
	UpdateView();
}

void Camera::SetRotation(const DirectX::XMFLOAT3A& newRot)
{
	cameraRotation = newRot;
	UpdateView();
}

void Camera::MoveRotation(const DirectX::XMFLOAT3A& newRot)
{
	cameraRotation += newRot;
	UpdateView();
}
//End of XMFLOAT Functions--------------------------------------------------------------//

//XMVECTOR Functions----------------------------------------------------------------------//
void Camera::SetPositionVec(const DirectX::XMVECTOR& newPos)
{
	cameraPositionVec = newPos;
	UpdateView();
}

void Camera::MovePositionVec(const DirectX::XMVECTOR& newPos)
{
	cameraPositionVec = DirectX::XMVectorAdd(cameraPositionVec, newPos);
	UpdateView();
}

void Camera::SetRotationVec(const DirectX::XMVECTOR& newRot)
{
	cameraRotationVec = newRot;
	UpdateView();
}

void Camera::MoveRotationVec(const DirectX::XMVECTOR& newRot)
{
	cameraRotationVec = DirectX::XMVectorAdd(cameraRotationVec, newRot);
	UpdateView();
}
//End of XMVECTOR Functions--------------------------------------------------------------//

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

	///Note: XMVECTOR Method
	forwardVec = DirectX::XMLoadFloat3A(&DirectX::XMFLOAT3A(0.f, cameraRotation.y, 1.f));
	rightVec = DirectX::XMLoadFloat3A(&DirectX::XMFLOAT3A(1.f, cameraRotation.y, 0.f));
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

	//Apply roll to camera rotation
	SetRoll();
}

void Camera::LookAt(DirectX::XMFLOAT3A target)
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

	//Apply roll to camera rotation
	SetRoll();
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

	//Apply roll to camera rotation
	SetRoll();
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
		yaw += PI;

	cameraRotation.y = yaw;
}

void Camera::SetRoll()
{
	///Calculate Roll ()		TODO
}

void Camera::MoveCameraForwardD()
{
	forwardDir.x *= cameraSpeed;
	forwardDir.y *= cameraSpeed;
	forwardDir.z *= cameraSpeed;

	MovePosition(forwardDir);

	///Note: XMVECTOR Method
	/*
	forwardVec = DirectX::XMVectorMultiply(forwardVec, DirectX::XMVectorSet(cameraSpeed, cameraSpeed, cameraSpeed, 0.f));
	MovePositionVec(forwardVec);
	*/
}

void Camera::MoveCameraBackwardsD()
{
	//Reverse / Negative of the forward direction
	forwardDir.x *= -1.f * cameraSpeed;
	forwardDir.y *= -1.f * cameraSpeed;
	forwardDir.z *= -1.f * cameraSpeed;

	MovePosition(forwardDir);

	///Note: XMVECTOR Method
	/*
	forwardVec = DirectX::XMVectorMultiply(forwardVec, DirectX::XMVectorSet(-cameraSpeed, -cameraSpeed, -cameraSpeed, 0.f));
	MovePositionVec(forwardVec);
	*/
}

void Camera::MoveCameraLeftD()
{
	//Reverse / Negative of the right direction
	rightDir.x *= -1.f * cameraSpeed;
	rightDir.y *= -1.f * cameraSpeed;
	rightDir.z *= -1.f * cameraSpeed;

	MovePosition(rightDir);

	///Note: XMVECTOR Method
	/*
	rightVec = DirectX::XMVectorMultiply(rightVec, DirectX::XMVectorSet(-cameraSpeed, -cameraSpeed, -cameraSpeed, 0.f));
	MovePositionVec(rightVec);
	*/
}

void Camera::MoveCameraRightD()
{
	rightDir.x *= cameraSpeed;
	rightDir.y *= cameraSpeed;
	rightDir.z *= cameraSpeed;

	MovePosition(rightDir);

	///Note: XMVECTOR Method
	/*
	rightVec = DirectX::XMVectorMultiply(rightVec, DirectX::XMVectorSet(cameraSpeed, cameraSpeed, cameraSpeed, 0.f));
	MovePositionVec(rightVec);
	*/
}

const void Camera::MoveCameraUpwards()
{
	this->MovePosition(DirectX::XMFLOAT3A(0.f, 0.01f, 0.f));
}

const void Camera::MoveCameraDownwards()
{
	this->MovePosition(DirectX::XMFLOAT3A(0.f, -0.01f, 0.f));
}

const void Camera::MoveCameraLeft()
{
	this->MovePosition(DirectX::XMFLOAT3A(-0.01f, 0.f, 0.f));
}

const void Camera::MoveCameraRight()
{
	this->MovePosition(DirectX::XMFLOAT3A(0.01f, 0.f, 0.f));
}

const void Camera::ZoomIn()
{
	this->MovePosition(DirectX::XMFLOAT3A(0.f, 0.f, 0.01f));
}

const void Camera::ZoomOut()
{
	this->MovePosition(DirectX::XMFLOAT3A(0.f, 0.f, -0.01f));
}

const void Camera::SpinCameraAround()
{
	this->MoveRotation(DirectX::XMFLOAT3A(0.f, 0.f, 0.01f));
}
