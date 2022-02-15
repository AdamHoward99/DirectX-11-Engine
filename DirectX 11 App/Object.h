#pragma once
#include <d3d11.h>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

/*
Object class is used to load Objects geometry from an Object file (.obj, .fbx, etc.) and pass information such as attached textures and mesh data to the Mesh class.
Object file importing is done via Assimp version 4.5.1.
This class is used by the GameObject class and uses the Mesh class. Structure of these classes is to keep low-level object generation hidden from the surface (GameObject class).
Developer -> GameObject -> Object -> Mesh
*/

class Object
{
public:
	///Constructor to create the Object class' geometry via the Mesh class
	const Object(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, const std::string& filepath);
	///Copy Constructor
	const Object(const Object& oldObject);
	///Move Constructor
	const Object(Object&& oldObject);
	///Class Destructor
	~Object();
	///Copy Assignment Operator
	Object& operator=(const Object& oldObject);
	///Move Assignment Operator
	Object& operator=(Object&& oldObject);
	///Called in constructor, used for assigning Meshes to the Object
	const void Initialize(const std::string& filepath);
	///Updates transformations for Object every frame
	const void Update();
	///Draws the Object in the game world using Mesh class
	const void Render();
	///Obtains Object geometry from file in given filepath
	const void CreateObjGeometry(const std::string& filepath);
	///Obtains all nodes from Object geometry and passes nodes to Mesh class
	const void ProcessNodes(const aiScene* pScene, const aiNode* node);
	///Obtains all vertices and indices from Object geometry + attached texture information, passes this to Mesh class
	const Mesh ProcessMeshes(const aiScene* pScene, const aiMesh* mesh);
	///Obtains the textures attached to Object geometry via stored filepath
	const void LoadMaterialTexture(const aiScene* pScene, const aiMaterial* pMat, const aiTextureType texType, std::vector<Texture>& textures) const;
	///Finds texture filepath based on method of storage in Object file
	const Texture GetTextureByStorageType(const aiScene* pScene, const aiTextureType texType, const aiString* texStr) const;
	///Changes position of object in game world by multiplying with current world matrix
	const void SetWorldPosition(const DirectX::XMMATRIX& pos);
	///Gets the world position of the Object
	const DirectX::XMMATRIX& GetWorldPosition() const;
	///Sets position of Object according to camera viewport (Camera View * Camera Projection)
	const void SetViewProjectionMatrix(const DirectX::XMMATRIX& viewProjMatrix);
	///Assigns a new material to the Object
	const void AssignNewMaterial(const Material& mat);
	///Sets the diffuse albedo values for the material on the mesh
	const void SetMaterialDiffuseAlbedo(const DirectX::XMFLOAT4& diffuse);
	///Sets the fresnel effect values for the material on the mesh
	const void SetMaterialFresnel(const DirectX::XMFLOAT3A& fresnel);
	///Overloaded version of SetMaterialFresnel function passing in individual floats
	const void SetMaterialFresnel(const float fresnelX, const float fresnelY, const float fresnelZ);
	///Sets the roughness for the material on the mesh
	const void SetMaterialRoughness(const float roughness);

private:
	///Passed in via GameObject class constructor, used  to pass into the Mesh class when creating Objects geometry
	Microsoft::WRL::ComPtr<ID3D11Device> pObjDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pObjDeviceContext;

	///Matrix used to calculate the WVP matrix, is passed to Mesh class each Update call
	DirectX::XMMATRIX objWorldMatrix = DirectX::XMMatrixIdentity();
	///Matrix used to calculate the WVP matrix, is passed to Mesh class and calculated in Mesh::UpdatePosition function
	DirectX::XMMATRIX viewProjectionMatrix = DirectX::XMMatrixIdentity(); 
	///Vector of Meshes found when loading the Object file
	std::vector<Mesh> objMeshes;
	///String location of the Object file in the local directory
	std::string objectFileDirectory;

	///Value of Objects materials albedo values, can be changed in GameObject class, alters appearance of material
	DirectX::XMFLOAT4 meshMaterialDiffuseAlbedo = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	///Values of Objects materials fresnel effect, can be changed in GameObject class, is the percentage of light that a surface reflects in the eye angle
	DirectX::XMFLOAT3A meshMaterialFresnel = DirectX::XMFLOAT3A(0.01f, 0.01f, 0.01f);
	///Value of Objects materials roughness value, can be changed in GameObject class, lower values represent shinier materials
	float meshMaterialRoughness = 1.f; 

	/*
	Material Variable Rules:

	Fresnel Values: Range is [0.001f - 1.f]
	The higher fresnel values will produce brighter and more condensed light reflected back into the eye position.
	The lower fresnel values will produce lower and dimmed light reflected back into the eye position.

	//Roughness Value: Range is [0.01f - 1.f]
	A higher roughness value will produce more condensed reflection lighting.
	A lower roughness value will produce more scattered reflection lighting.
	*/
};