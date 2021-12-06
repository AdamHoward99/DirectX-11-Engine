#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera();

	void SetProjection(float deg, float rat, float nearZ, float farZ);

	void SetPosition(const DirectX::XMFLOAT3& pos);
	void MovePosition(const DirectX::XMFLOAT3& pos);

	void SetRotation(const DirectX::XMFLOAT3& rot);
	void MoveRotation(const DirectX::XMFLOAT3& rot);

	const DirectX::XMFLOAT3& GetPosition() const { return cameraPosition; }
	const DirectX::XMFLOAT3& GetRotation() const { return cameraRotation; }

	const DirectX::XMMATRIX& GetCameraView() const { return cameraView; }
	const DirectX::XMMATRIX& GetProjection() const { return projection; }

	//Engine Example Functions
	const void MoveCameraUpwards();
	const void MoveCameraDownwards();
	const void MoveCameraLeft();
	const void MoveCameraRight();
	const void ZoomIn();
	const void ZoomOut();
private:
	void UpdateView();

	DirectX::XMMATRIX cameraView;
	DirectX::XMMATRIX projection;

	DirectX::XMFLOAT3 cameraPosition;
	DirectX::XMFLOAT3 cameraRotation;

	//TODO: Include both XMVECTOR and XMFLOAT3 Versions of the variables?
	//TODO: Add keyboard controls for DEBUG camera?
};