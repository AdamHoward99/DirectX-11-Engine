#include "Object.h"

Object::Object(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, const std::string& filepath = "")
{
	pObjDevice = pDevice;
	pObjDeviceContext = pDeviceContext;
	Initialize(filepath);
}

Object::~Object()
{

}

void* Object::operator new(size_t i)
{
	return _mm_malloc(i, 16);		//Aligns to 16 for unique_ptr creation
}

void Object::operator delete(void * p)
{
	_mm_free(p);
}

void Object::Initialize(const std::string& filepath)
{
	if (filepath != "")		//Draw default triangle for now
		CreateObjGeometry(filepath);
	else
		objMeshes.push_back(Mesh(pObjDevice, pObjDeviceContext));		//Creates triangle when no OBJ has been chosen
}

void Object::Update()
{
	//Update Position of OBJ Here
	objMeshes[0].UpdatePosition(objWorldMatrix);
	Render();
}

void Object::Render()
{
	//Draw Meshes for this Obj
	for (Mesh& m : objMeshes)
		m.Draw();
}

void Object::CreateObjGeometry(const std::string& filepath)
{
	//Create OBJ Importer
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	//TODO: MAKE INTO UNIQUE_PTR OR SMART POINTER
	if (pScene == nullptr)
		exit(-1);		//TODO: CHANGE TO BOOL FUNCTION RETURNS FALSE TO CREATE OBJ BUFFERS OR INITIALIZE

	//Process Nodes
	ProcessNodes(pScene, pScene->mRootNode);
}

void Object::ProcessNodes(const aiScene* pScene, const aiNode* node)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = pScene->mMeshes[node->mMeshes[i]];
		objMeshes.push_back(ProcessMeshes(pScene, mesh));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNodes(pScene, node->mChildren[i]);
	}
}

Mesh Object::ProcessMeshes(const aiScene* pScene, const aiMesh* mesh)
{
	std::vector<Vertex> verts;
	std::vector<DWORD> inds;

	//Get vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex v;
		v.vertexPosition.x = mesh->mVertices[i].x;
		v.vertexPosition.y = mesh->mVertices[i].y;
		v.vertexPosition.z = mesh->mVertices[i].z;
		//TODO: COULD DO OVERLOAD OPERATOR FOR +

		if (mesh->mTextureCoords[0])		//Check if this mesh has a texture, main texture is always 1st texture
		{
			v.textureCoordinates.x = (float)mesh->mTextureCoords[0][i].x;
			v.textureCoordinates.y = (float)mesh->mTextureCoords[0][i].y;
		}

		verts.push_back(v);
	}

	//Get the index Data
	for (UINT i = 0; i < mesh->mNumFaces; i++)		//Faces should be triangle based on triangulate value
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			inds.push_back(face.mIndices[j]);
	}

	return Mesh(pObjDevice, pObjDeviceContext, verts, inds);
}

void Object::SetWorldPosition(const DirectX::XMMATRIX & pos)
{
	objWorldMatrix = pos;
	objWorldMatrix = DirectX::XMMatrixTranspose(objWorldMatrix);
}

const DirectX::XMMATRIX& Object::GetWorldPosition() const
{
	return objWorldMatrix;
}

void Object::LoadMeshTexture(const std::wstring filename)
{
	for (auto& m : objMeshes)
		m.LoadTexture(filename);
}
