#pragma once
#include "stb_image.h"

class Texture {
public:
  unsigned int ID;
  Texture(const char *texturePath);

private:
  void GenerateTexture(const char *path, const int &width, const int &height,
                       const int &nChannels, unsigned char *data);
};
