#include "assert_vertex.h"
#include "callBack.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "project_init.h"
#include "shader.h"
#include "stb_image.h"
#include "texture.h"
#include "tools.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  glfwProjectInit();
  GLFWwindow *window = windowCreate(SCR_WIDTH, SCR_HEIGHT);
  gladInit(window, SCR_WIDTH, SCR_HEIGHT);

  global_OpenglState();

  // build and compile our shader program
  // ------------------------------------
  Shader vertex("./shader/coordinate1/vertex.glsl", 0);
  Shader fragment("./shader/coordinate1/frag.glsl", 1);

  ShaderP shaderProgram(vertex, fragment);

  float *vertices = cube_vertices();
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 36 * 5 * sizeof(float), vertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  delete vertices;
  // texture
  Texture texture1("./assert/container.jpg");
  Texture texture2("./assert/awesomeface.png");

  // activate texture unit in shaderProgram
  shaderProgram.use();
  shaderProgram.setInt("texture1", 0);
  shaderProgram.setInt("texture2", 1);

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.use();

    // coordinate
    float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    glm::mat4 view;
    view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),
                       glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection =
        glm::perspective(glm::radians(45.0f),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shaderProgram.set4Matrix("view", view);
    shaderProgram.set4Matrix("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1.ID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2.ID);

    glBindVertexArray(VAO);

    for (int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, static_cast<float>(glfwGetTime()),
                          glm::vec3(1.0f, 0.3f, 0.5f));
      shaderProgram.set4Matrix("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
