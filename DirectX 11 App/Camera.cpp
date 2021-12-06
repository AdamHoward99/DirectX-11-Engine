#include "Camera.h"

Camera::Camera()
{
	//Set Default Values
	cameraPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	cameraRotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	UpdateView();
}

//+= Overload for XMFLOAT3
const DirectX::XMFLOAT3& operator+=(DirectX::XMFLOAT3& A, const DirectX::XMFLOAT3& B)
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

void Camera::SetPosition(const DirectX::XMFLOAT3& newPos)
{
	cameraPosition = newPos;
	UpdateView();
}

void Camera::MovePosition(const DirectX::XMFLOAT3& newPos)
{
	cameraPosition += newPos;
	UpdateView();
}

void Camera::SetRotation(const DirectX::XMFLOAT3& newRot)
{
	cameraRotation = newRot;
	UpdateView();
}

void Camera::MoveRotation(const DirectX::XMFLOAT3& newRot)
{
	cameraRotation += newRot;
	UpdateView();
}

void Camera::UpdateView()
{
	//Calculate camera rotation matrix
	DirectX::XMMATRIX cameraRotMatrix = DirectX::XMMatrixRotationRollPitchYaw(cameraRotation.x, cameraRotation.y, cameraRotation.z);

	//Calculate unit vector
	DirectX::XMVECTOR cameraTarget = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), cameraRotMatrix);

	//Adjust camera target
	DirectX::XMVectorAdd(cameraTarget, DirectX::XMLoadFloat3(&cameraPosition));

	//Calculate Up direction based on current rotation
	DirectX::XMVECTOR upDirection = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f), cameraRotMatrix);

	//Rebuild view matrix
	cameraView = DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&cameraPosition), cameraTarget, upDirection);
}

const void Camera::MoveCameraUpwards()
{
	this->MovePosition(DirectX::XMFLOAT3(0.f, -0.01f, 0.f));
}

const void Camera::MoveCameraDownwards()
{
	this->MovePosition(DirectX::XMFLOAT3(0.f, 0.01f, 0.f));
}

const void Camera::MoveCameraLeft()
{
	this->MovePosition(DirectX::XMFLOAT3(0.01f, 0.f, 0.f));
}

const void Camera::MoveCameraRight()
{
	this->MovePosition(DirectX::XMFLOAT3(-0.01f, 0.f, 0.f));
}

const void Camera::ZoomIn()
{
	this->MovePosition(DirectX::XMFLOAT3(0.f, 0.f, 0.01f));
}

const void Camera::ZoomOut()
{
	this->MovePosition(DirectX::XMFLOAT3(0.f, 0.f, -0.01f));
}
