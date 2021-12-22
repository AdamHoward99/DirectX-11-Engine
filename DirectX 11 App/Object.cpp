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
	///Model filename given in DXGraphics::InitialiseOBJs()
	if (filepath != "")
	{
		CreateObjGeometry(filepath);
		return;
	}
	
	///Creates default triangle model when no OBJ filename has been given
	objMeshes.push_back(Mesh(pObjDevice, pObjDeviceContext));
}

void Object::Update()
{
	//Update Position of OBJ Here
	for (Mesh& m : objMeshes)
		m.UpdatePosition(objWorldMatrix);

	Render();
}

void Object::Render()
{
	///Sets Texture of Object in PixelShader.hlsl
	//pObjDeviceContext->PSSetShaderResources(0, 1, pTexture.GetAddressOf());
	///PSSetShaderResources(IN, IN, OPTIONAL)
	///UINT StartSlot			 - Index of array to begin setting resources
	///UINT NumViews			 - Amount of shader resources required to be set up, maximum of 128
	///ID3D11ShaderResourceView* - Array of SRV interfaces to set to device

	//Draw Meshes for this Obj
	for (Mesh& m : objMeshes)
		m.Draw();
}

void Object::CreateObjGeometry(const std::string& filepath)
{
	///Create OBJ Importer to obtain Model file
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	///ReadFile(IN, IN)
	///const string& pFile - File path to 3D Model file
	///unsigned int pFlags - Flags to determine transformations made to 3D Model data upon importing
	
	///FLAGS:
	///aiProcess_ConvertToLeftHanded: Shortcut flag for DirectX 3D-based applications.
	///aiProcess_Triangulate: Separates faces with more than 3 indices into triangles.

	///If File failed to load or was not found
	if (pScene == nullptr)
		exit(-1);		//TODO: CHANGE TO BOOL FUNCTION RETURNS FALSE TO CREATE OBJ BUFFERS OR INITIALIZE

	//Process Nodes
	ProcessNodes(pScene, pScene->mRootNode);
}

void Object::ProcessNodes(const aiScene* pScene, const aiNode* node)
{
	///Loops through and obtains all Meshes of the current Node
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = pScene->mMeshes[node->mMeshes[i]];
		objMeshes.push_back(ProcessMeshes(pScene, mesh));
	}

	///Obtains all Meshes and Nodes of the current Nodes children
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNodes(pScene, node->mChildren[i]);
	}
}

Mesh Object::ProcessMeshes(const aiScene* pScene, const aiMesh* mesh)
{
	std::vector<Vertex> verts;
	std::vector<DWORD> inds;

	///Obtains all vertices for each Mesh, storing in vector
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex v;
		v.vertexPosition.x = mesh->mVertices[i].x;
		v.vertexPosition.y = mesh->mVertices[i].y;
		v.vertexPosition.z = mesh->mVertices[i].z;
		//TODO: COULD DO OVERLOAD OPERATOR FOR +

		///If this Mesh has a texture preset, obtain the texture coordinates	NOTE: Main texture of Mesh is always the 1st texture
		if (mesh->mTextureCoords[0])
		{
			v.textureCoordinates.x = (float)mesh->mTextureCoords[0][i].x;
			v.textureCoordinates.y = (float)mesh->mTextureCoords[0][i].y;
		}

		verts.push_back(v);
	}

	///Obtains all indices for each face of the Mesh, all Faces will be set as triangles due to aiProcess_Triangulate flag
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		///Obtains indices of each Face
		for (UINT j = 0; j < face.mNumIndices; j++)
			inds.push_back(face.mIndices[j]);
	}

	//Way of testing textures outputting TODO: Remove after testing
	std::vector<Texture> textures;
	textures.push_back(Texture(pObjDevice, aiColor4D(100.f, 100.f, 100.f, 255.f), aiTextureType::aiTextureType_DIFFUSE));

	///Calls the OBJ Constructor of Mesh which creates Index and Vertex buffers based on the imported Vertices and Indices values.
	return Mesh(pObjDevice, pObjDeviceContext, verts, inds, textures);
}

void Object::SetWorldPosition(const DirectX::XMMATRIX& pos)
{
	///World Matrix value is passed onto Mesh class where the position is actually set each Update()
	objWorldMatrix = pos;

	///Transposed as DirectX uses a column-major coordinate system
	objWorldMatrix = DirectX::XMMatrixTranspose(objWorldMatrix);
}

const DirectX::XMMATRIX& Object::GetWorldPosition() const
{
	return objWorldMatrix;
}

void Object::LoadMeshTexture(const std::wstring& filename)
{
	//HRESULT hr = DirectX::CreateWICTextureFromFile(pObjDevice.Get(), filename.c_str(), nullptr, pTexture.GetAddressOf());
	///CreateWICTextureFromFile(IN, IN, IN, IN, OPTIONAL)
	///ID3D11Device* d3dDevice				  - Pointer to the device using this resource
	///const wchar_t* fileName				  - C string pointer to texture file path
	///ID3D11Resource** texture				  - Pointer to the pointer of the texture resource 
	///ID3D11ShaderResourceView** textureView - Pointer to the pointer of the texture SRV
	///size_t maxsize						  - Maximum size of the texture file

	//if (FAILED(hr))
	//	ErrorMes::DisplayErrMessage(hr);
}
