#include "assert_vertex.h"
#include "callBack.h"
#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "project_init.h"
#include "shader.h"
#include "shape/cube.h"
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

float lastX = static_cast<float>(SCR_WIDTH) / 2;
float lastY = static_cast<float>(SCR_HEIGHT) / 2;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;

// time
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
int main() {
  glfwProjectInit();
  GLFWwindow *window = windowCreate(SCR_WIDTH, SCR_HEIGHT);
  gladInit(window, SCR_WIDTH, SCR_HEIGHT);

  global_OpenglState();

  // callback : mouse and keyboard
  project_init_callback(window);

  // build and compile our shader program
  // ------------------------------------
  Shader vertex("./shader/light/vertex.glsl", 0);
  Shader cube_fragment("./shader/light/fragment.glsl", 1);
  Shader lighting_fragment("./shader/light/lighting.glsl", 1);

  ShaderP cube_shaderProgram(vertex, cube_fragment);
  ShaderP lighting_shaderProgram(vertex, lighting_fragment);

  vertex.unUsed();
  cube_fragment.unUsed();
  lighting_fragment.unUsed();

  float *vertices = cube_vertices();

  Cube cube(vertices, cube_state::without_texture);

  delete vertices;

  glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cube_shaderProgram.use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::mat4(1.0f);
    projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    cube_shaderProgram.set4Matrix("view", view);
    cube_shaderProgram.set4Matrix("projection", projection);
    cube_shaderProgram.set4Matrix("model", model);
    cube_shaderProgram.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    cube_shaderProgram.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    cube.use();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lighting_shaderProgram.use();
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lighting_shaderProgram.set4Matrix("view", view);
    lighting_shaderProgram.set4Matrix("projection", projection);
    lighting_shaderProgram.set4Matrix("model", model);
    cube.use();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
