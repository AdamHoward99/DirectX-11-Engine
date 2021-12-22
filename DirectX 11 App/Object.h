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

	///Required for 16-bit alignment of Object class for unordered_map in DXGraphics Class
	void* operator new(size_t i);
	void operator delete(void* p);

	void Initialize(const std::string& filepath);
	void Update();
	void Render();

	///Functions to obtain 3D Model data from OBJ file
	void CreateObjGeometry(const std::string& filepath);
	void ProcessNodes(const aiScene* pScene, const aiNode* node);
	Mesh ProcessMeshes(const aiScene* pScene, const aiMesh* mesh);

	void SetWorldPosition(const DirectX::XMMATRIX& pos);
	const DirectX::XMMATRIX& GetWorldPosition() const;

	void LoadMeshTexture(const std::wstring& filename);		//TODO: Combine this with Mesh class for better UV'ing

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pObjDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pObjDeviceContext;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;

	DirectX::XMMATRIX objWorldMatrix;	///Matrix is passed to Meshes to set world position
	std::vector<Mesh> objMeshes;

	/*TODO: in future, GameObject class will inherit Object class adding:
	position variables
	rotation variables
	scaling variables
	collision
	etc.
	*/
};