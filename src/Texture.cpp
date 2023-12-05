#include "texture.h"
#include "sstream"
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>

Texture::Texture(const char *texturePath) {
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nChannels;
  unsigned char *data = stbi_load(texturePath, &width, &height, &nChannels, 0);

  if (data) {
    GenerateTexture(texturePath, width, height, nChannels, data);
  } else {
    std::cout << "TEXTURE :: image load failed" << std::endl;
  }
  stbi_image_free(data);
}

void Texture::GenerateTexture(const char *path, const int &width,
                              const int &height, const int &nChannels,
                              unsigned char *data) {
  std::stringstream ss(path);
  std::vector<std::string> s_list;
  std::string item;
  while (getline(ss, item, '.')) {
    s_list.push_back(item);
  }
  std::string type = s_list.back();

  if (!type.compare("jpg")) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else if (!type.compare("png")) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "ERROR :: TEXTURE :: unknow file type" << std::endl;
  }
}
