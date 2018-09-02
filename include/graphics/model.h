
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <graphics/common.h>
#include <graphics/texture.h>
#include <assimp\Importer.hpp>

#ifndef SHUKI_MODEL
#define SHUKI_MODEL

namespace graphics {

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define TANGENT_LOCATION 3
#define BONE_ID_LOCATION 4
#define BONE_WEIGHT_LOCATION 5

#define INVALID_MATERIAL 0xFFFFFFFF

#define NUM_BONES_PER_VERTEX 4

struct Mesh {
  Mesh() {
    NumIndices = 0;
    BaseVertex = 0;
    BaseIndex = 0;
    MaterialIndex = INVALID_MATERIAL;
  }

  unsigned int NumIndices;
  unsigned int BaseVertex;
  unsigned int BaseIndex;
  unsigned int MaterialIndex;
};

class _ModelChunk {
 public:
  void load();

 private:
  std::string model_file;
  float* vertices_data;
};

struct VertexBoneData {
  uint32_t IDs[NUM_BONES_PER_VERTEX];
  float weights[NUM_BONES_PER_VERTEX];
  void AddBoneData(size_t BoneID, float Weight);
};

struct BoneInfo {
  aiMatrix4x4 FinalTransformation;
  aiMatrix4x4 BoneOffset;
};

#define BUFFER_INDEX 0
#define BUFFER_POS 1
#define BUFFER_NORMAL 2
#define BUFFER_TEXCORD 3
#define BUFFER_WVP_mat 4
#define BUFFER_TANGENT 5
#define BUFFER_VB 6

#define BUFFER_NUM 7
class Model {
 public:
  Model(std::string& name_);

  void Init();

  bool InitfromScene(const aiScene* pScene);

  void InitMesh(const aiMesh* pMesh, std::vector<vmath::vec3>& positions,
                std::vector<vmath::vec3>& normals,
                std::vector<vmath::vec2>& texd_uvs,
                std::vector<unsigned int>& indices,
                std::vector<vmath::vec3>& tangents);

  bool InitMaterials(const aiScene* pScene, const std::string& Filename);

  aiMatrix4x4 BoneTransform(float TimeInSeconds,
                            std::vector<aiMatrix4x4>& transforms);

  void Render();

  void RenderwithoutTexture();

  ~Model();

 private:
  bool InitformScene(const aiScene* pScene, const std::string& Filename);

  void LoadBones(size_t MeshIndex, const aiMesh* pMesh,
                 std::vector<VertexBoneData>& Bones);

  void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode,
                         const aiMatrix4x4& ParentTranform);

  size_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);

  size_t FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

  size_t FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

  void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime,
                                const aiNodeAnim* pNodeAnim);

  void CalcInterpolatedPositoin(aiVector3D& Out, float AnimationTime,
                                const aiNodeAnim* pNodeAnim);

  void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime,
                               const aiNodeAnim* pNodeAnim);

  GLuint m_vao;
  GLuint m_buffers[BUFFER_NUM];
  std::shared_ptr<_ModelChunk> _modeldata;
  std::string filename;

  std::unique_ptr<Assimp::Importer> m_importer;
  const ::aiScene* pScene = nullptr;
  std::vector<Mesh> m_meshes;
  std::vector<Texture*> m_diffuse_textures;
  std::vector<Texture*> m_normal_textures;
  std::map<std::string, size_t> m_BoneMapping;
  std::vector<BoneInfo> m_BoneInfo;

  aiMatrix4x4 m_GlobalInverseTransform;
};

}  // namespace graphics

#endif  // !SHUKI_MODEL