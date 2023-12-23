#pragma once
#include "shader.h"
#include <assimp/types.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};
struct Texture_i {
  unsigned int id;
  std::string type;
  aiString path;
};

class BasicMesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture_i> textures;
  BasicMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
            std::vector<Texture_i> textures);

  void render(ShaderP shader);

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};
