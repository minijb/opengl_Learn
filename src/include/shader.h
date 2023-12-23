#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  unsigned int shaderID;
  Shader(const char *shaderPath, const int &opt);
  void unUsed();
};

class ShaderP {
public:
  // 程序ID
  unsigned int ID;

  // 构造器读取并构建着色器
  ShaderP(Shader vertex, Shader fragment);
  // 使用/激活程序
  void use();
  // uniform工具函数
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void set4Float(const std::string &name, float v1, float v2, float v3,
                 float v4) const;
  void set4Matrix(const std::string &name, glm::mat4 value) const;
  void setVec3(const std::string &name, glm::vec3 vec3) const;
  void setVec3(const std::string &name, float v1, float v2, float v3) const;
  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif
