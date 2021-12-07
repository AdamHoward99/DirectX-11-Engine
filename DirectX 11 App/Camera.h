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
	void LookAt(const DirectX::XMMATRIX& targetPos);
	void LookAt(DirectX::XMFLOAT3A targetPos);
	void LookAt(float x, float y, float z);

	//XMFLOAT3 Functions
	const DirectX::XMFLOAT3A& GetPosition() const { return cameraPosition; }
	void SetPosition(const DirectX::XMFLOAT3A& pos);
	void MovePosition(const DirectX::XMFLOAT3A& pos);

	const DirectX::XMFLOAT3A& GetRotation() const { return cameraRotation; }
	void SetRotation(const DirectX::XMFLOAT3A& rot);
	void MoveRotation(const DirectX::XMFLOAT3A& rot);

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
	DirectX::XMFLOAT3A cameraPosition;
	DirectX::XMFLOAT3A cameraRotation;

	//XMVECTOR Variables
	DirectX::XMVECTOR cameraPositionVec;
	DirectX::XMVECTOR cameraRotationVec;

	static int CamerasInUse;
	int CameraNo;

	///Notice: Aligned variables can be more efficient in cases where bits aren't aligned.
	///Notice: Don't overuse Operator Overloads for types like XMMATRIX and XMVECTOR, they create multiple temp objs.
	//TODO: Add keyboard controls for DEBUG camera?
};
