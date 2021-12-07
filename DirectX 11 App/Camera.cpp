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
	DirectX::XMVectorAdd(cameraPositionVec, newPos);
	UpdateView();
}

void Camera::SetRotationVec(const DirectX::XMVECTOR& newRot)
{
	cameraRotationVec = newRot;
	UpdateView();
}

void Camera::MoveRotationVec(const DirectX::XMVECTOR& newRot)
{
	DirectX::XMVectorAdd(cameraRotationVec, newRot);
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
}

void Camera::LookAt(const DirectX::XMVECTOR& cameraPos, const DirectX::XMMATRIX& targetPos)
{
	//TODO: work on this, not sure if working properly yet



	//Convert XMMATRIX to XMVECTOR3
	DirectX::XMVECTOR targetPosVector = DirectX::XMVector3Transform(DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f), targetPos);

	//Normalized vector of distance between camera and target
	DirectX::XMVECTOR L = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetPosVector, cameraPos));

	//Normalized vector of cross product of up direction and L
	DirectX::XMVECTOR R = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSet(0.f, 0.1f, 0.f, 0.f), L));

	//Position Look at
	DirectX::XMStoreFloat3A(&cameraPosition, cameraPos);
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
