#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera();

	///Engine Example Functions - Uses XMFLOAT by default
	const void MoveCameraUpwards();
	const void MoveCameraDownwards();
	const void MoveCameraLeft();
	const void MoveCameraRight();
	const void ZoomIn();
	const void ZoomOut();

	const DirectX::XMMATRIX& GetCameraView() const { return cameraView; }
	const DirectX::XMMATRIX& GetProjection() const { return projection; }

	const int GetCameraNumber() const { return CameraNo; }

	void SetProjection(float deg, float rat, float nearZ, float farZ);
	void LookAt(const DirectX::XMVECTOR& cameraPos, const DirectX::XMMATRIX& targetPos);	//TODO: work in progress

	//XMFLOAT3 Functions
	const DirectX::XMFLOAT3& GetPosition() const { return cameraPosition; }
	void SetPosition(const DirectX::XMFLOAT3& pos);
	void MovePosition(const DirectX::XMFLOAT3& pos);

	const DirectX::XMFLOAT3& GetRotation() const { return cameraRotation; }
	void SetRotation(const DirectX::XMFLOAT3& rot);
	void MoveRotation(const DirectX::XMFLOAT3& rot);

	//XMVECTOR Functions
	const DirectX::XMVECTOR& GetPositionVec() const { return cameraPositionVec; }
	void SetPositionVec(const DirectX::XMVECTOR& pos);
	void MovePositionVec(const DirectX::XMVECTOR& pos);

	const DirectX::XMVECTOR& GetRotationVec() const { return cameraRotationVec; }
	void SetRotationVec(const DirectX::XMVECTOR& rot);
	void MoveRotationVec(const DirectX::XMVECTOR& rot);

private:
	void UpdateView();		///Function uses XMFLOAT by default

	DirectX::XMMATRIX cameraView;
	DirectX::XMMATRIX projection;

	//XMFLOAT Variables
	DirectX::XMFLOAT3 cameraPosition;
	DirectX::XMFLOAT3 cameraRotation;

	//XMVECTOR Variables
	DirectX::XMVECTOR cameraPositionVec;
	DirectX::XMVECTOR cameraRotationVec;

	static int CamerasInUse;
	int CameraNo;

	//TODO: Add keyboard controls for DEBUG camera?
};

int Camera::CamerasInUse = 0;		//Counts total cameras used across all scenes