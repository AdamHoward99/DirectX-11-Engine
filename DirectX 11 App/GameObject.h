#pragma once
#include "Object.h"

/*
	Since this engine is using the Left-Handed Coordinate System, the Y and Z values are reversed.
	Diagram of a Left-Handed Coordinate System:
	Z
	|
	|
	|
	|__ __ __ __X
	 \
	  \
	   \
	    \
	     Y

	All parameter inputs for transformation functions such as SetPosition, SetRotation etc, will be passed as XYZ. The DirectX11 operations will then swap
	the Y and Z values and store in the order of XZY. I.e passing (1.f, 2.f, 1.f) would be stored as (1.f, 1.f, 2.f) by the engine.

	When passing a parameter by the Y or Z value, expect for the opposite value to be affected (apart from translations but this is being looked at).
	For example, calling SetScale with (1.f, 2.f, 1.f) as parameters will scale the model in the Z axis, NOT the Y axis.

	For all Get functions regarding transformations, these will return values according to the right-handed coordinate system. For easier remembering, if
	you call SetRotation with parameters (0.f, 90.f, 0.f) and then call GetRotationY; it will return 90. The Y and Z get functions will obtain information from the opposite
	axis position. This change was made to make it easier to understand what axis is being changed since most associate with the right-handed coordinate system.

	Setting the axis uses Right-Hand (XYZ) -> DirectX Operations change order to Left-Hand (XZY) -> Getting axis value converts back to Right-Hand (XYZ)
*/

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
	///XMVECTOR version of GetPosition function
	const DirectX::XMVECTOR GetPositionVec();
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
	///XMVECTOR version of GetRotation function
	const DirectX::XMVECTOR GetRotationVec();
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
	///XMVECTOR version of GetScale function
	const DirectX::XMVECTOR GetScaleVec();
	///XMFLOAT3A version of GetScale function
	const DirectX::XMFLOAT3A GetScale3A();
	///Obtains X scale value from Object world matrix
	const float GetScaleX();
	///Obtains Y scale value from Object world matrix
	const float GetScaleY();
	///Obtains Z scale value from Object world matrix
	const float GetScaleZ();

	///Required for 16-bit alignment of GameObject class
	void* operator new(size_t i);
	///Frees memory allocated in unique_pointer declaration
	void operator delete(void* p);

	//look at function
	//rotation around point 
	//Collision functions?


	///Sets position of Object based on Camera View
	void SetViewProjectionMatrix(const DirectX::XMMATRIX& viewProjMatrix);
private:
	///Mesh of the GameObject, Keeps Mesh loading and Geometry creation hidden from this class
	Object object;
	///Stores name used by unordered_map in DXGraphics class
	std::string gameObjectName;
};