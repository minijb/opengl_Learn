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
#include "tools.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Model.hpp"
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = static_cast<float>(SCR_WIDTH) / 2;
float lastY = static_cast<float>(SCR_HEIGHT) / 2;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// time
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

GLFW_UserPoint glfw_userpoint;

int main() {

  // ========= project init : glfw and glad=========
  glfwProjectInit();
  GLFWwindow *window = windowCreate(SCR_WIDTH, SCR_HEIGHT);
  gladInit(window, SCR_WIDTH, SCR_HEIGHT);

  global_OpenglState();

  // ========= callback : mouse and keyboard =========
  project_init_callback(window);

  // ========= build and compile our shader program =========
  Shader vertex("./shader/model/vertex.glsl", 0);
  Shader fragment("./shader/model/fragment.glsl", 1);

  // shaderprogram
  ShaderP model_shaderProgram(vertex, fragment);

  // delete shader
  vertex.unUsed();
  fragment.unUsed();

  // ========= get cube shape --- with VAO =========
  Model our_model("./assert/model/nanosuit/nanosuit.obj");

  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);
    glfwPollEvents();

    // Rendering

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model_shaderProgram.use();
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    model_shaderProgram.setMat4("projection", projection);
    model_shaderProgram.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    model_shaderProgram.setMat4("model", model);
    our_model.Draw(model_shaderProgram);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
