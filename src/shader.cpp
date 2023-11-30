#include "shader.h"
#include "tools.h"
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
