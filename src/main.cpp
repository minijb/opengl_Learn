#include "callBack.h"
#include "project_init.h"
#include "shader.h"
#include "tools.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <math.h>
#include <string>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  glfwProjectInit();
  GLFWwindow *window = windowCreate(SCR_WIDTH, SCR_HEIGHT);
  gladInit(window, SCR_WIDTH, SCR_HEIGHT);

  // build and compile our shader program
  // ------------------------------------
  Shader vertex("./shader/vertex.glsl", 0);
  Shader fragment("./shader/frag.glsl", 1);

  ShaderP shaderProgram(vertex, fragment);

  float vertices[] = {
      0.5f,  -0.5f, 0.0f, // 右下
      -0.5f, -0.5f, 0.0f, // 左下
      0.0f,  0.5f,  0.0f, // 顶部
  };
  unsigned int indices[] = {0, 1, 2};
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use();

    glBindVertexArray(VAO);

    float timeValue = glfwGetTime();
    float greenValue = static_cast<float>(sin(timeValue) / 2.0f + 0.5f);
    shaderProgram.set4Float("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
