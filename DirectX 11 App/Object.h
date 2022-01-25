#pragma once
#include <d3d11.h>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

class Object			///Class to load OBJs, textures and Meshes
{
public:
	Object(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, const std::string& filepath);
	~Object();

	///Copy Constructor for Object class
	Object(const Object& oldObject);

	///Copy Assignment Operator for Object class
	Object& operator=(const Object& oldObject);

	void Initialize(const std::string& filepath);
	void Update(const DirectX::XMFLOAT3A&);
	void Render();

	///Functions to obtain 3D Model data from OBJ file
	void CreateObjGeometry(const std::string& filepath);
	void ProcessNodes(const aiScene* pScene, const aiNode* node);
	Mesh ProcessMeshes(const aiScene* pScene, const aiMesh* mesh);
	const void LoadMaterialTexture(const aiScene* pScene, const aiMaterial* pMat, const aiTextureType texType, std::vector<Texture>& textures);
	Texture GetTextureByStorageType(const aiScene* pScene, const aiTextureType texType, const aiString* texStr);

	///Changes position of object in game world by multiplying with current world matrix
	void SetWorldPosition(const DirectX::XMMATRIX& pos);
	const DirectX::XMMATRIX& GetWorldPosition() const;

	///Sets position of Object according to camera viewport (Camera View * Camera Projection)
	void SetViewProjectionMatrix(const DirectX::XMMATRIX& viewProjMatrix);

	///Sets the fresnel for the material on the mesh
	void SetMaterialFresnel(const DirectX::XMFLOAT3A& fresnel);
	void SetMaterialFresnel(const float fresnelX, const float fresnelY, const float fresnelZ);

	///Sets the roughness for the material on the mesh
	void SetMaterialRoughness(const float roughness);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pObjDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pObjDeviceContext;

	DirectX::XMMATRIX objWorldMatrix = DirectX::XMMatrixIdentity();				///Matrix is passed to Meshes to set world position
	DirectX::XMMATRIX viewProjectionMatrix = DirectX::XMMatrixIdentity();		///Used for VP Matrix, multipled with world matrix in Mesh::UpdatePosition function 
	std::vector<Mesh> objMeshes;
	std::string objectFileDirectory;

	///Variables for Mesh material variables (Fresnel, Roughness)
	DirectX::XMFLOAT3A meshMaterialFresnel = DirectX::XMFLOAT3A(0.01f, 0.01f, 0.01f);
	float meshMaterialRoughness = 1.f;
};