#pragma once
#include "BasicMesh.hpp"
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image.h>

#define ASSIMP_MODEL_LOAD_FLAGS aiProcess_Triangulate | aiProcess_FlipUVs

class Model {
public:
  /*  函数   */
  Model(char *path);
  void Draw(ShaderP shader);

private:
  /*  模型数据  */
  std::vector<BasicMesh> meshes;
  std::vector<Texture_i> textures_loaded;
  std::string directory;
  /*  函数   */
  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  BasicMesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture_i> loadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              std::string typeName);

  unsigned int TextureFromFile(const char *path, const std::string &directory);
};
