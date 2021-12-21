#pragma once
#include <d3d11.h>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <WICTextureLoader.h>
#include "Mesh.h"

class Object			//Model
{
public:
	Object(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, const std::string& filepath);
	~Object();

	void* operator new(size_t i);
	void operator delete(void* p);

	void Initialize(const std::string& filepath);
	void Update();
	void Render();

	void CreateObjGeometry(const std::string& filepath);
	void ProcessNodes(const aiScene* pScene, const aiNode* node);
	Mesh ProcessMeshes(const aiScene* pScene, const aiMesh* mesh);

	void SetWorldPosition(const DirectX::XMMATRIX& pos);
	const DirectX::XMMATRIX& GetWorldPosition() const;

	void LoadMeshTexture(const std::wstring filename);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pObjDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pObjDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;


	DirectX::XMMATRIX objWorldMatrix;
	std::vector<Mesh> objMeshes;		//COULD ONLY HAVE A SINGLE MESH VARIABLE INSTEAD OF VECTOR, CURRENT MODELS ONLY SEEM TO HAVE SINGLE MESH
	/*TODO: in future, GameObject class will inherit Object class adding:
	position variables
	rotation variables
	scaling variables
	collision
	etc.
	*/
};