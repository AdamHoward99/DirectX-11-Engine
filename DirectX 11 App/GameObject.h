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
	///Matrix version of SetPosition function
	void SetPosition(const DirectX::XMMATRIX& newPosition);
	///XMVECTOR version of SetPosition function
	void SetPosition(const DirectX::XMVECTOR& newPosition);
	///XMFLOAT4X4A version of SetPosition function
	void SetPosition(const DirectX::XMFLOAT4X4A& newPosition);
	///XMFLOAT3A version of SetPosition function
	void SetPosition(const DirectX::XMFLOAT3A& newPosition);
	///Singular floats version of SetPosition function
	void SetPosition(const float x, const float y, const float z);
	///Sets position of Object based on Camera View
	void SetViewProjectionMatrix(const DirectX::XMMATRIX& viewProjMatrix);
	///Matrix version of GetPosition function
	const DirectX::XMMATRIX& GetPositionMatrix();
	///XMFLOAT4X4A version of GetPosition function
	const DirectX::XMFLOAT4X4A GetPosition4X4();
	///XMFLOAT3A version of GetPosition function
	const DirectX::XMFLOAT3A GetPosition3A();
	///Obtains X position from Object world matrix
	const float GetPositionX();
	///Obtains Y position from Object world matrix
	const float GetPositionY();
	///Obtains Z position from Object world matrix
	const float GetPositionZ();

	///Required for 16-bit alignment of GameObject class
	void* operator new(size_t i);
	///Frees memory allocated in unique_pointer declaration
	void operator delete(void* p);

private:
	///Mesh of the GameObject, Keeps Mesh loading and Geometry creation hidden from this class
	Object object;
	///Stores name used by unordered_map in DXGraphics class
	std::string gameObjectName;
};