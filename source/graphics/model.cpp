#include <graphics/model.h>
#include<string>

namespace graphics
{

Model::Model(std::string & name_)
{
	filename = name_;
}


void Model::Init()
{

	m_importer = std::make_unique<Assimp::Importer>(*new Assimp::Importer());
	pScene = m_importer->ReadFile(filename.c_str(), aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs
		| aiProcess_CalcTangentSpace);

	if (!pScene) {
		LOG("The object file doesn't exist");
		return;
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);
	InitfromScene(pScene);
	glBindVertexArray(0);

	//patching for animation
	if (pScene) {
		auto m_GlobalInverseTransform = pScene->mRootNode->mTransformation;
		m_GlobalInverseTransform.Inverse();
	}
	//

};

bool Model::InitfromScene(const aiScene* pScene)
{
	using namespace std;
	m_meshes.resize(pScene->mNumMeshes);
	m_diffuse_textures.resize(pScene->mNumMaterials);
	m_normal_textures.resize(pScene->mNumMaterials);
	std::vector<vmath::vec3>   positions;
	std::vector<vmath::vec3>   normals;
	std::vector<vmath::vec2>   texd_uvs;
	std::vector<vmath::vec3>   tangents;
	std::vector<unsigned int> indices;

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
	{
		m_meshes[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_meshes[i].BaseVertex = NumVertices;
		m_meshes[i].BaseIndex = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_meshes[i].NumIndices;

	}

	positions.reserve(NumVertices);
	normals.reserve(NumVertices);
	texd_uvs.reserve(NumVertices);
	tangents.reserve(NumVertices);
	indices.reserve(NumIndices);

	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		const aiMesh* pmesh = pScene->mMeshes[i];
		InitMesh(pmesh, positions, normals, texd_uvs, indices, tangents);
	}


	if (pScene->mNumMaterials && !InitMaterials(pScene, filename)) {
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BUFFER_POS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BUFFER_TEXCORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texd_uvs[0]) * texd_uvs.size(), &texd_uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BUFFER_NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[BUFFER_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BUFFER_TANGENT]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangents[0]) * tangents.size(), &tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TANGENT_LOCATION);
	glVertexAttribPointer(TANGENT_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//patching for animation
	std::vector<VertexBoneData> Bones;
	Bones.resize(NumVertices);
	for (size_t mesh_i = 0; mesh_i < m_meshes.size(); mesh_i++) {
		LoadBones(mesh_i, pScene->mMeshes[mesh_i], Bones);
	}

		
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BUFFER_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

		
	//


	return GLCheckError();
}

void Model::InitMesh(const aiMesh * pMesh,
	std::vector<vmath::vec3> &positions,
	std::vector<vmath::vec3> &normals,
	std::vector<vmath::vec2> &texd_uvs,
	std::vector<unsigned int> &indices,
	std::vector<vmath::vec3> &tangents)
{

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
		auto pPos = &(pMesh->mVertices[i]);
		auto pNormal = &(pMesh->mNormals[i]);
		auto pTexUV = pMesh->HasTextureCoords(0) ? &(pMesh->mTextureCoords[0][i]) : &Zero3D;
		auto pTangent = &(pMesh->mTangents[i]);
		positions.push_back(vmath::vec3(pPos->x, pPos->y, pPos->z));
		normals.push_back(vmath::vec3(pNormal->x, pNormal->y, pNormal->z));
		texd_uvs.push_back(vmath::vec2(pTexUV->x, pTexUV->y));
		tangents.push_back(vmath::vec3(pTangent->x, pTangent->y, pTangent->z));
	}

	for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
		const aiFace & face = pMesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

}


bool Model::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;


	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_diffuse_textures[i] = NULL;
		m_normal_textures[i] = NULL;
		using namespace std;
		if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {
			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string p(Path.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				m_normal_textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_normal_textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_normal_textures[i];
					m_normal_textures[i] = NULL;
					//      Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string p(Path.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				m_diffuse_textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_diffuse_textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_diffuse_textures[i];
					m_diffuse_textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}
	}

	return Ret;
}

void Model::Render() {
	glBindVertexArray(m_vao);
	for (unsigned int i = 0; i < m_meshes.size(); i++) {
		const unsigned int MaterialIndex = m_meshes[i].MaterialIndex;

		//_ASSERT(MaterialIndex < m_textures.size());



		if (m_diffuse_textures[MaterialIndex]) {
			m_diffuse_textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);
		}
		if (m_normal_textures[MaterialIndex]) {
			m_normal_textures[MaterialIndex]->Bind(NORMAL_TEXTURE_UNIT);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_meshes[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_meshes[i].BaseIndex),
			m_meshes[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}


//RenderwithoutTexuture is special for the purpose that render a
//plane for display the content of the previou shader's output.
void Model::RenderwithoutTexture() {
	glBindVertexArray(m_vao);
	for (unsigned int i = 0; i < m_meshes.size(); i++) {
		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_meshes[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_meshes[i].BaseIndex),
			m_meshes[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);

}
 Model::~Model(){

}


 void Model::LoadBones(size_t MeshIndex, const aiMesh * pMesh, std::vector<VertexBoneData>& Bones) {
	 for (size_t i = 0; i < pMesh->mNumBones; i++) {
		 size_t BoneIndex = 0;
		 std::string BoneName(pMesh->mBones[i]->mName.data);

		 if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			 BoneIndex = m_BoneInfo.size();
			 BoneInfo bi;
			 m_BoneInfo.push_back(bi);
		 }
		 else {
			 BoneIndex = m_BoneMapping[BoneName];
		 }

		 m_BoneMapping[BoneName] = BoneIndex;
		 m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;

		 for (size_t j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
			 size_t VertexID = m_meshes[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			 float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			 Bones[VertexID].AddBoneData(BoneIndex, Weight);
		 }
	 }

 }


 void VertexBoneData::AddBoneData(size_t BoneID, float Weight) {
	 for (size_t i = 0; i < NUM_BONES_PER_VERTEX; i++) {
		 if (weights[i] == 0.0) {
			 IDs[i] = BoneID;
			 weights[i] = Weight;
			 return;
		 }
	 }
	
	 _ASSERT(0);
 }

 aiMatrix4x4 Model::BoneTransform(float TimeInSeconds, std::vector<aiMatrix4x4> & transforms) {
	 aiMatrix4x4 Identity = aiMatrix4x4();
	
	 float TicksPerSecond = pScene->mAnimations[0]->mTicksPerSecond != 0 ?
		 pScene->mAnimations[0]->mTicksPerSecond : 25.0f;
	 float TimeInTicks = TimeInSeconds * TicksPerSecond;
	 float AnimationTime = fmod(TimeInTicks, pScene->mAnimations[0]->mDuration);
	 ReadNodeHeirarchy(AnimationTime, pScene->mRootNode, Identity);
	 size_t bones_size = m_BoneInfo.size();
	 transforms.resize(bones_size);
	 for (size_t i = 0; i < bones_size; i++) {
		 transforms[i] = m_BoneInfo[i].FinalTransformation;
	 }
	 return aiMatrix4x4();
  }


 aiNodeAnim* FindNodeAnim(const aiAnimation * pAnimation, std::string NodeName) {
	 size_t sz = pAnimation->mNumChannels;
	 for (int i = 0; i < sz; i++) {
		 if (pAnimation->mChannels[i]->mNodeName.C_Str() == NodeName) {
			 return pAnimation->mChannels[i];
		 }
	 }
	 _ASSERT(0);
 }

 void Model::ReadNodeHeirarchy(float AnimationTime, const aiNode * pNode, const aiMatrix4x4 & ParentTranform) {
	 std::string NodeName(pNode->mName.data);

	 const aiAnimation * pAnimation = pScene->mAnimations[0];

	 aiMatrix4x4 NodeTransformation(pNode->mTransformation);
	 const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation,NodeName);

	 if (pNodeAnim) {
		 aiVector3D scaling;
		 CalcInterpolatedScaling(scaling, AnimationTime, pNodeAnim);
		 aiMatrix4x4 scalingM;
		 
		 aiQuaternion RotationQ;
		 CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		 aiMatrix4x4 rotationM = aiMatrix4x4(RotationQ.GetMatrix());

		 aiVector3D Translation;
		 CalcInterpolatedPositoin(Translation, AnimationTime, pNodeAnim);
		 aiMatrix4x4 TranslationM;


		 NodeTransformation = TranslationM * rotationM * scalingM;
		 aiMatrix4x4 GlobalTransformation = ParentTranform * NodeTransformation;

		 if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
			 size_t BoneIndex = m_BoneMapping[NodeName];
			 m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
		 }
		 for (size_t i = 0; i < pNode->mNumChildren; i++) {
			 ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
		 }
	 }
 }

 void Model::CalcInterpolatedRotation(aiQuaternion & Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
 
	 if (pNodeAnim->mNumRotationKeys == 1) {
		 Out = pNodeAnim->mRotationKeys[0].mValue;
		 return;
	 }
	 size_t RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	 size_t NextRotationIndex = (RotationIndex + 1);
	 _ASSERT(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	 float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime 
		 - pNodeAnim->mRotationKeys[RotationIndex].mTime;
	 float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) /
		 DeltaTime;
	 _ASSERT(Factor >= 0.0f && Factor < 1.0f);

	 const aiQuaternion & StartRoationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	 const aiQuaternion & EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;

	 aiQuaternion::Interpolate(Out, StartRoationQ, EndRotationQ, Factor);

	 Out = Out.Normalize();
 }

 void Model::CalcInterpolatedPositoin(aiVector3D & Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
	 if (pNodeAnim->mNumPositionKeys == 1) {
		 Out = pNodeAnim->mPositionKeys[0].mValue;
		 return;
	 }
	 size_t PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	 size_t NextPositionIndex = PositionIndex + 1;
	 _ASSERT(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	 float DeltaTime = pNodeAnim->mPositionKeys[NextPositionIndex].mTime
		 - pNodeAnim->mPositionKeys[PositionIndex].mTime;
	 float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) /
		 DeltaTime;
	 _ASSERT(Factor >= 0.0f && Factor < 1.0f);

	 const aiVector3D & StartPositionQ = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	 const aiVector3D & EndPositionQ = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;

	 Out = StartPositionQ * Factor + EndPositionQ * (1 - Factor);
 }
 

 void Model::CalcInterpolatedScaling(aiVector3D & Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
	 if (pNodeAnim->mNumPositionKeys == 1) {
		 Out = pNodeAnim->mScalingKeys[0].mValue;
		 return;
	 }
	 size_t ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	 size_t NextScalingIndex = ScalingIndex + 1;
	 _ASSERT(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	 float DeltaTime = pNodeAnim->mScalingKeys[NextScalingIndex].mTime 
		 - pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
	 float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) /
		 DeltaTime;
	 _ASSERT(Factor >= 0.0f && Factor < 1.0f);
	
	 auto & StartScalingQ = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	 auto & EndScalingQ = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;

	 Out = StartScalingQ * Factor + EndScalingQ * (1 - Factor);
 
 }


 size_t Model::FindScaling(float AnimationTime, const aiNodeAnim * pNodeAnim) {
	 _ASSERT(pNodeAnim->mNumScalingKeys > 0);

	 for (size_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		 if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			 return i;
		 }
	 }
	 _ASSERT(0);
 }

 size_t Model::FindPosition(float AnimationTime, const aiNodeAnim * pNodeAnim) {
	 _ASSERT(pNodeAnim->mNumPositionKeys > 0);
	
	 for (size_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		 if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			 return i;
		 }
	 }
	 _ASSERT(0);
 }

 size_t Model::FindRotation(float AnimationTime, const aiNodeAnim * pNodeAnim) {
	 _ASSERT(pNodeAnim->mNumRotationKeys > 0);

	 for (size_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		 if (AnimationTime < (float)pNodeAnim->mRotationKeys[i+1].mTime){
			 return i;
		 }
	 }
	 _ASSERT(0);
 }


}



