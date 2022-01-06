#pragma once
#include "Object.h"

class GameObject
{
public:
	///Constructor to initiliaze GameObject's model object
	GameObject(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, const std::string& filepath, 
		const std::string& gameObjectName = "Default_GameObject");
	///GameObject Class Destructor
	virtual ~GameObject();
	///Copy Assignment Operator
	virtual GameObject& operator=(const GameObject& otherObj);
	///Move Assignment Operator
	virtual GameObject& operator=(const GameObject&& otherObj);

	///Updates Movement of GameObject
	virtual void Update();
	///Matrix version of SetTransformations function, usable for any XMMATRIX transformation
	void SetTransformations(const DirectX::XMMATRIX& newMatrix);
	///XMFLOAT4X4A version of SetTransformations function, usable for any XMFLOAT4X4A transformation
	void SetTransformations(const DirectX::XMFLOAT4X4A& newF4X4);
	///Matrix version of GetTransformations function
	const DirectX::XMMATRIX& GetTransformationsMatrix();
	///XMFLOAT4X4A version of GetTransformations function
	const DirectX::XMFLOAT4X4A GetTransformations4X4();
	///XMVECTOR version of SetPosition function
	void SetPosition(const DirectX::XMVECTOR& newPosition);
	///XMFLOAT3A version of SetPosition function
	void SetPosition(const DirectX::XMFLOAT3A& newPosition);
	///Singular floats version of SetPosition function
	void SetPosition(const float x, const float y, const float z);
	///XMFLOAT3A version of GetPosition function
	const DirectX::XMFLOAT3A GetPosition3A();
	///Obtains X position from Object world matrix
	const float GetPositionX();
	///Obtains Y position from Object world matrix
	const float GetPositionY();
	///Obtains Z position from Object world matrix
	const float GetPositionZ();
	///XMVECTOR version of SetRotation function
	void SetRotation(const DirectX::XMVECTOR& newRotation);
	///XMFLOAT3A version of SetRotation function
	void SetRotation(const DirectX::XMFLOAT3A& newRotation);
	///Singular floats version of SetRotation function
	void SetRotation(const float x, const float y, const float z);
	///XMFLOAT3A version of GetRotation function
	const DirectX::XMFLOAT3A GetRotation3A();
	///Obtains X rotation from Object world matrix
	const float GetRotationX();
	///Obtains Y rotation from Object world matrix
	const float GetRotationY();
	///Obtains Z rotation from Object world matrix
	const float GetRotationZ();
	///XMVECTOR version of SetScale function
	void SetScale(const DirectX::XMVECTOR& newScale);
	///XMFLOAT3A version of SetScale function
	void SetScale(const DirectX::XMFLOAT3A& newScale);
	///Singular float version of SetScale function
	void SetScale(const float x, const float y, const float z);

	///Required for 16-bit alignment of GameObject class
	void* operator new(size_t i);
	///Frees memory allocated in unique_pointer declaration
	void operator delete(void* p);

	//TODO : Get for XMVECTOR?

	///Sets position of Object based on Camera View
	void SetViewProjectionMatrix(const DirectX::XMMATRIX& viewProjMatrix);
private:
	///Mesh of the GameObject, Keeps Mesh loading and Geometry creation hidden from this class
	Object object;
	///Stores name used by unordered_map in DXGraphics class
	std::string gameObjectName;
};