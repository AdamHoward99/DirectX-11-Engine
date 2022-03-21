#pragma once
#include <DirectXMath.h>

#define PI DirectX::XM_PI

//The Camera class uses the Left-Handed Coordinate System like the GameObject class does, therefore the Y and Z values are reversed.
//If the camera is to have a physical object in the engine in the future, derive from the GameObject class.

class Camera
{
public:
	///Default Constructor, sets default values for Camera position and rotation
	Camera();
	///Constructor to set Camera position and rotation, uses XMFLOAT3A variables
	Camera(const DirectX::XMFLOAT3A& camPos, const DirectX::XMFLOAT3A& camRot);
	///Constructor to set Camera position and rotation, uses XMVECTOR variables
	Camera(const DirectX::XMVECTOR& camPos, const DirectX::XMVECTOR& camRot);
	///Copy Constructor for Camera class
	Camera(const Camera& otherCam);
	///Move Constructor for Camera class
	Camera(Camera&& otherCam);
	///Default Destructor for Camera class
	~Camera();
	///Copy Assignment Operator for Camera class
	Camera& operator=(const Camera& otherCam);
	///Move Assignment Operator for Camera class
	Camera& operator=(Camera&& otherCam);
	///XMFLOAT3A version of SetPosition function
	void SetPosition(const DirectX::XMFLOAT3A& pos);
	///XMVECTOR version of SetPosition function
	void SetPosition(const DirectX::XMVECTOR& pos);
	///Singular float version of SetPosition function
	void SetPosition(const float X, const float Y, const float Z);
	///XMFLOAT3A version of GetPosition function
	const DirectX::XMFLOAT3A& GetPosition3A() const;
	///XMVECTOR version of GetPosition function
	const DirectX::XMVECTOR& GetPositionVec() const;
	///Returns X value of Cameras position
	const float GetPositionX() const;
	///Returns Y value of Cameras position
	const float GetPositionY() const;
	///Returns Z value of Cameras position
	const float GetPositionZ() const;
	///XMFLOAT3A version of SetRotation function
	void SetRotation(const DirectX::XMFLOAT3A& rot);
	///XMVECTOR version of SetRotation function
	void SetRotation(const DirectX::XMVECTOR& rot);
	///Singular float version of SetRotation function
	void SetRotation(const float X, const float Y, const float Z);
	///XMFLOAT3A version of MoveRotation function, rotates from Cameras current rotation via the Engine class
	void AddToRotation(const DirectX::XMFLOAT3A& rot);
	///XMFLOAT3A version of MoveRotation function
	void AddToRotation(const DirectX::XMVECTOR& rot);
	///XMFLOAT3A version of MoveRotation function
	void AddToRotation(const float X, const float Y, const float Z);
	///XMFLOAT3A version of GetRotation function
	const DirectX::XMFLOAT3A& GetRotation3A() const;
	///XMVECTOR version of GetRotation function
	const DirectX::XMVECTOR& GetRotationVec() const;
	///Returns X value of Cameras rotation
	const float GetRotationX() const;
	///Returns Y value of Cameras rotation
	const float GetRotationY() const;
	///Returns Z value of Cameras rotation
	const float GetRotationZ() const;
	///Debug Camera Forward Movement for XMFLOAT3A
	void MoveCameraForward3A_D(const float dt);
	///Debug Camera Forward Movement for XMVECTOR
	void MoveCameraForwardVec_D(const float dt);
	///Debug Camera Backwards Movement for XMFLOAT3A
	void MoveCameraBackwards3A_D(const float dt);
	///Debug Camera Backwards Movement for XMVECTOR
	void MoveCameraBackwardsVec_D(const float dt);
	///Debug Camera Left Movement for XMFLOAT3A
	void MoveCameraLeft3A_D(const float dt);
	///Debug Camera Left Movement for XMVECTOR
	void MoveCameraLeftVec_D(const float dt);
	///Debug Camera Right Movement for XMFLOAT3A
	void MoveCameraRight3A_D(const float dt);
	///Debug Camera Right Movement for XMVECTOR
	void MoveCameraRightVec_D(const float dt);
	///Debug Camera Elevate Movement for XMFLOAT3A
	void ElevateCamera3A_D(const float dt);
	///Debug Camera Elevate Movement for XMVECTOR
	void ElevateCameraVec_D(const float dt);
	///Debug Camera Lower Movement for XMFLOAT3A
	void LowerCamera3A_D(const float dt);
	///Debug Camera Lower Movement for XMVECTOR
	void LowerCameraVec_D(const float dt);

	//zoom in  / out functions
	//set camera speed
	//set mouse camera movement

	void SetupCamera(const DirectX::XMFLOAT3A& startPos, const float aspRatio, const float nearZ, const float farZ);

	///Engine Example Functions - Uses XMFLOAT by default
	//const void ZoomIn();
	//const void ZoomOut();

	const DirectX::XMMATRIX& GetCameraView() const { return cameraView; }
	const DirectX::XMMATRIX& GetProjection() const { return projection; }

	const int GetCameraNumber() const { return CameraNo; }

	void SetProjection(float deg, float rat, float nearZ, float farZ);
	void LookAt(const DirectX::XMMATRIX& targetPos);
	void LookAt(DirectX::XMFLOAT3A targetPos);
	void LookAt(float x, float y, float z);

	//Euler Angle Calculations
	void SetPitch(const float y, const float distance);
	void SetYaw(const float x, const float z);





	void SetMouseMovement(bool foo);
	bool GetMouseMovement() const;

private:

	///XMFLOAT3A version of MovePosition function, moves from Cameras current position via the Debug Movement functions
	void AddToPosition(const DirectX::XMFLOAT3A& pos);
	///XMVECTOR version of MovePosition function
	void AddToPosition(const DirectX::XMVECTOR& pos);
	///Singular float version of MovePosition function
	void AddToPosition(const float X, const float Y, const float Z);

	void UpdateView();		///Function uses XMFLOAT by default

	DirectX::XMMATRIX cameraView;
	DirectX::XMMATRIX projection;

	//XMFLOAT Variables
	DirectX::XMFLOAT3A cameraPosition;
	DirectX::XMFLOAT3A cameraRotation;

	DirectX::XMFLOAT3A forwardDir = DirectX::XMFLOAT3A(0.f, 0.f, 1.f);
	DirectX::XMFLOAT3A rightDir = DirectX::XMFLOAT3A(1.f, 0.f, 0.f);
	DirectX::XMFLOAT3A upDir = DirectX::XMFLOAT3A(0.f, 1.f, 0.f);

	//XMVECTOR Variables
	DirectX::XMVECTOR cameraPositionVec;
	DirectX::XMVECTOR cameraRotationVec;

	DirectX::XMVECTOR forwardVec = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
	DirectX::XMVECTOR rightVec = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
	DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
	
	const float cameraSpeed = 0.002f;
	static int CamerasInUse;
	int CameraNo;

	//Projection Variables
	float aspectRatio;
	float nearZ;
	float farZ;

	bool mouseMovementEnabled = false;

	///Notice: Aligned variables can be more efficient in cases where bits aren't aligned.
	///Notice: Don't overuse Operator Overloads for types like XMMATRIX and XMVECTOR, they create multiple temp objs.
};
