#include "Object.h"

Object::Object(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, const std::string& filepath = "")
{
	pObjDevice = pDevice;
	pObjDeviceContext = pDeviceContext;
	Initialize(filepath);
}

Object::~Object()
{}

Object::Object(const Object& oldObject)
{
	pObjDevice = oldObject.pObjDevice;
	pObjDeviceContext = oldObject.pObjDeviceContext;
	objMeshes = oldObject.objMeshes;
	objectFileDirectory = oldObject.objectFileDirectory;
	objWorldMatrix = oldObject.objWorldMatrix;
}

Object& Object::operator=(const Object& oldObject)
{
	///Ensure this is not a self-assignment
	if (this != &oldObject)
	{
		this->pObjDevice = oldObject.pObjDevice;
		this->pObjDeviceContext = oldObject.pObjDeviceContext;
		this->objMeshes = oldObject.objMeshes;
		this->objectFileDirectory = oldObject.objectFileDirectory;
		this->objWorldMatrix = oldObject.objWorldMatrix;
	}

	return *this;
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
	///Update Position of OBJ here
	for (Mesh& m : objMeshes)
		m.UpdatePosition(objWorldMatrix, viewProjectionMatrix);

	Render();
}

void Object::Render()
{
	///Draw Meshes for OBJ here
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
		ErrorMes::DisplayErrorMessage(__LINE__, "Assimp Importer::ReadFile(): failed to load file", __FILE__);

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
	std::vector<Texture> ObjMaterialTextures;

	///Loops through to obtain all texture types used by mesh
	for(int i = 1; i < 12; i++)
		LoadMaterialTexture(pScene, mat, static_cast<aiTextureType>(i), ObjMaterialTextures);

	///Texture types by index
	/*
		1  = aiTextureType_DIFFUSE      - Main texture of object
		2  = aiTextureType_SPECULAR     - Strength of specular reflection
		3  = aiTextureType_AMBIENT      - Ambient texture, does not incorporate lighting
		4  = aiTextureType_EMISSIVE     - Receives no lighting so texture is unaffected by incoming light
		5  = aiTextureType_HEIGHT       - Uses gray scale values to show higher elevations on a material
		6  = aiTextureType_NORMALS      - Bump maps used for smaller bumps
		7  = aiTextureType_SHININESS    - Sharpness of the reflections in the material, also called roughness
		8  = aiTextureType_OPACITY	    - Opacity of the material, black for transparency, white for opaque
		9  = aiTextureType_DISPLACEMENT - Bump maps for larger bumps such as terrain
		10 = aiTextureType_LIGHTMAP     - Pronounced detail in material with shadowing
		11 = aiTextureType_REFLECTION   - States where reflection should and shoudn't be, not used frequently
	*/

	///Calls the OBJ Constructor of Mesh which creates Index and Vertex buffers based on the imported Vertices and Indices values.
	return Mesh(pObjDevice, pObjDeviceContext, verts, inds, ObjMaterialTextures);
}

void Object::SetWorldPosition(const DirectX::XMMATRIX& pos)
{
	///World Matrix value is passed onto Mesh class where the position is actually set each Update()
	objWorldMatrix *= pos;
}

const DirectX::XMMATRIX& Object::GetWorldPosition() const
{
	return objWorldMatrix;
}

void Object::SetViewProjectionMatrix(const DirectX::XMMATRIX& viewProjMatrix)
{
	///View Projection Matrix is calculated in Mesh class with World Matrix
	viewProjectionMatrix = viewProjMatrix;
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
			std::string texPath = path.C_Str();

			///Check how the texture is stored to determine how it should be loaded in
			textures.push_back(GetTextureByStorageType(pScene, texType, &path));
		}
	}

	///Give default texture in event of unhandled events
	if(textures.size() == 0)
		textures.push_back(Texture(pObjDevice, aiColor4D(255.f, 255.f, 255.f, 255.f), aiTextureType::aiTextureType_DIFFUSE));
}

Texture Object::GetTextureByStorageType(const aiScene* pScene, const aiTextureType texType, const aiString* texStr)
{
	///Function to create texture correctly depending on how it is stored (On Disk, Compressed Indexed, Non Compressed Index)
	const std::string textureStr = texStr->C_Str();

	///Compressed Indexed Check (Indexed textures always have * as the starting char)
	if (textureStr[0] == '*')
	{
		if (textureStr.size() < 2)
			ErrorMes::DisplayErrorMessage(__LINE__, "Texture material index string has size less than 2", __FILE__);

		///Get the index of the texture being used by this mesh
		int textureIndex = std::atoi(&textureStr[1]);
		return Texture(pObjDevice, reinterpret_cast<uint8_t*>(pScene->mTextures[textureIndex]->pcData), pScene->mTextures[textureIndex]->mWidth, texType);	///Creates an Embedded Compressed Texture
	}

	///On Disk Check (Texture file is located in same folder as the model file)
	if (textureStr.find('.') != std::string::npos)
		return Texture(pObjDevice, objectFileDirectory + textureStr, texType);

	///If it has gotten to this point, throw an error
	__assume(false);
}
