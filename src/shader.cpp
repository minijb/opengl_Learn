#include "shader.h"
#include "glm/ext/vector_float3.hpp"
#include "tools.h"
#include <glm/glm.hpp>
#include <string>
Shader::Shader(const char *shaderPath, const int &opt) {
  // get code of shader
  std::string code = read_shader_file(std::string(shaderPath));
  const char *vShaderCode = code.c_str();

  // compile shader
  int success;
  char infoLog[512];
  std::string cl;

  if (opt == 0) {
    shaderID = glCreateShader(GL_VERTEX_SHADER);
    cl = std::string("VERTEX");
  } else {
    shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    cl = std::string("FRAGMENT");
  }
  glShaderSource(shaderID, 1, &vShaderCode, NULL);
  glCompileShader(shaderID);
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    std::cout << "ERROR :: SHADER :: " << cl << " :: COMPILE ERROR" << infoLog
              << std::endl;
  }
}

void Shader::unUsed() { glDeleteShader(shaderID); }

ShaderP::ShaderP(Shader vertex, Shader fragment) {
  ID = glCreateProgram();
  glAttachShader(ID, vertex.shaderID);
  glAttachShader(ID, fragment.shaderID);

  glLinkProgram(ID);

  // check
  int success;
  char infoLog[512];

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR :: SHADER_PROGRAMME :: LINKING_FAILED" << infoLog
              << std::endl;
  }
}

void ShaderP::use() { glUseProgram(ID); }

void ShaderP::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderP::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderP::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderP::set4Float(const std::string &name, float v1, float v2, float v3,
                        float v4) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}

void ShaderP::set4Matrix(const std::string &name, glm::mat4 value) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}
void ShaderP::setVec3(const std::string &name, glm::vec3 vec3) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec3));
}

void ShaderP::setVec3(const std::string &name, float v1, float v2,
                      float v3) const {
  setVec3(name, glm::vec3(v1, v2, v3));
}
void ShaderP::setMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderP::setMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderP::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
