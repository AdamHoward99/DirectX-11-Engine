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
	SetWorldPosition(DirectX::XMMatrixIdentity());

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
	//Draw Meshes for this Obj
	for (Mesh& m : objMeshes)
		m.Draw();
}

void Object::CreateObjGeometry(const std::string& filepath)
{
	///Store file directory to obtain linked textures of OBJ
	objectFileDirectory = filepath.substr(0, filepath.find_last_of('/') + 1);

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

	///Find the material that this mesh is using
	const aiMaterial* mat = pScene->mMaterials[mesh->mMaterialIndex];

	///Get Diffuse Textures
	std::vector<Texture> diffuseTexs;
	LoadMaterialTexture(pScene, mat, aiTextureType::aiTextureType_DIFFUSE, diffuseTexs);

	///Calls the OBJ Constructor of Mesh which creates Index and Vertex buffers based on the imported Vertices and Indices values.
	return Mesh(pObjDevice, pObjDeviceContext, verts, inds, diffuseTexs);
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

const void Object::LoadMaterialTexture(const aiScene* pScene, const aiMaterial* pMat, const aiTextureType texType, std::vector<Texture>& textures)
{
	///Obtain total amount of textures on Material
	unsigned int textureCount = pMat->GetTextureCount(texType);

	///GetTextureCount() returns 0 when no textures are attached to Material
	if (textureCount == 0)
	{
		aiColor4D aiColour(0.f, 0.f, 0.f, 0.f);
		///Determine the texture type
		switch (texType)
		{
		case aiTextureType_DIFFUSE:
			pMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiColour);		//Attempt to get the existing colour of the diffuse texture
			if (aiColour.IsBlack())			///Texture is empty
			{
				textures.push_back(Texture(pObjDevice, aiColor4D(0.f, 0.f, 100.f, 255.f), texType));
				return;
			}
			textures.push_back(Texture(pObjDevice, aiColor4D(aiColour.r * 255, aiColour.g * 255, aiColour.b * 255, aiColour.a * 255), texType));
			return;
			break;
		}
	} 
	else		///Object contains pre-existing attached Textures
	{	
		///For every texture linked to the model
		for (UINT i = 0; i < textureCount; i++)		
		{
			aiString path;
			pMat->GetTexture(texType, i, &path);

			///Create directory for the Image file
			std::string str = objectFileDirectory + path.C_Str();

			Texture tex(this->pObjDevice, str, texType);
			textures.push_back(tex);
		}
	}

	///Give default texture in event of unhandled events
	if(textures.size() == 0)
		textures.push_back(Texture(pObjDevice, aiColor4D(255.f, 255.f, 255.f, 255.f), aiTextureType::aiTextureType_DIFFUSE));
}
