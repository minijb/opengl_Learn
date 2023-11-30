#include "tools.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::string read_shader_file(std::string fileName) {
  std::ifstream shaderFile;
  std::stringstream sstring;
  std::string shaderCode;
  try {
    shaderFile.open(fileName);
    sstring << shaderFile.rdbuf();
    shaderCode = sstring.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR -- SHADER -- File not successfully read" << std::endl;
  }
  return shaderCode;
}
