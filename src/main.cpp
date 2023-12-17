#include "assert_vertex.h"
#include "callBack.h"
#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"
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

#include "imguiT.h"

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

  // ========= imgui init =========
  P_imgui::P_imguiInit(window);
  ImGuiIO &io = ImGui::GetIO();

  // ========= some useful val =========
  bool show_demo_window = true;

  // ========= build and compile our shader program =========
  Shader vertex("./shader/light/vertex_light.glsl", 0);
  Shader lighting("./shader/light/lighting_v.glsl", 0);
  Shader cube_fragment("./shader/light/fragment_lights.glsl", 1);
  Shader lighting_fragment("./shader/light/lighting.glsl", 1);

  // shaderprogram
  ShaderP cube_shaderProgram(vertex, cube_fragment);
  ShaderP lighting_shaderProgram(lighting, lighting_fragment);

  // delete shader
  vertex.unUsed();
  cube_fragment.unUsed();
  lighting_fragment.unUsed();
  lighting.unUsed();

  // ========= get cube shape --- with VAO =========
  float *vertices = cube_vertices_texture_texture();
  Cube cube(vertices, cube_state::with_normal_texture);
  delete vertices;

  Texture texture1("./assert/container2.png");
  Texture texture2("./assert/container2_specular.png");
  Texture texture3("./assert/matrix.jpg");

  // IMGUI control
  float emission_s = 0.5f;

  // Global var
  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);
    glfwPollEvents();

    P_imgui::P_Loop_init();

    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Light settings"); // Create a window called "Hello, world!"
      ImGui::SliderFloat("emission strength", &emission_s, 0.0f, 2.0f,
                         "strength = %.4f");
      ImGui::End();
    }

    // Rendering
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w,
                           &display_h); // 放置窗口拉伸导致的形变

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cube_shaderProgram.use();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::mat4(1.0f);
    projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)display_w / (float)display_h, 0.1f, 100.0f);
    cube_shaderProgram.set4Matrix("view", view);
    cube_shaderProgram.set4Matrix("projection", projection);
    // cube_shaderProgram.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    // cube_shaderProgram.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    // cube_shaderProgram.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    cube_shaderProgram.setFloat("material.shininess", 32.0f);
    cube_shaderProgram.setInt("material.diffuse", 0);
    cube_shaderProgram.setInt("material.specular", 1);
    cube_shaderProgram.setInt("material.emission", 2);
    cube_shaderProgram.setFloat("material.emission_strength", emission_s);

    cube_shaderProgram.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    cube_shaderProgram.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    cube_shaderProgram.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    cube_shaderProgram.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    cube_shaderProgram.setVec3("pointLights[0].position",
                               pointLightPositions[0]);
    cube_shaderProgram.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    cube_shaderProgram.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    cube_shaderProgram.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    cube_shaderProgram.setFloat("pointLights[0].constant", 1.0f);
    cube_shaderProgram.setFloat("pointLights[0].linear", 0.09f);
    cube_shaderProgram.setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    cube_shaderProgram.setVec3("pointLights[1].position",
                               pointLightPositions[1]);
    cube_shaderProgram.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    cube_shaderProgram.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    cube_shaderProgram.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    cube_shaderProgram.setFloat("pointLights[1].constant", 1.0f);
    cube_shaderProgram.setFloat("pointLights[1].linear", 0.09f);
    cube_shaderProgram.setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    cube_shaderProgram.setVec3("pointLights[2].position",
                               pointLightPositions[2]);
    cube_shaderProgram.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    cube_shaderProgram.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    cube_shaderProgram.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    cube_shaderProgram.setFloat("pointLights[2].constant", 1.0f);
    cube_shaderProgram.setFloat("pointLights[2].linear", 0.09f);
    cube_shaderProgram.setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    cube_shaderProgram.setVec3("pointLights[3].position",
                               pointLightPositions[3]);
    cube_shaderProgram.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    cube_shaderProgram.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    cube_shaderProgram.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    cube_shaderProgram.setFloat("pointLights[3].constant", 1.0f);
    cube_shaderProgram.setFloat("pointLights[3].linear", 0.09f);
    cube_shaderProgram.setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    cube_shaderProgram.setVec3("spotLight.position", camera.Position);
    // cube_shaderProgram.setVec3("light.position", camera.Position);
    // cube_shaderProgram.setFloat("light.cutOff",
    // glm::cos(glm::radians(12.5f)));
    // cube_shaderProgram.setFloat("light.outerCutOff",
    //                             glm::cos(glm::radians(17.5f)));
    // cube_shaderProgram.setFloat("light.constant", 1.0f);
    // cube_shaderProgram.setFloat("light.linear", 0.09f);
    // cube_shaderProgram.setFloat("light.quadratic", 0.032f);

    cube_shaderProgram.setVec3("camera.position", camera.Position);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1.ID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2.ID);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3.ID);

    for (int i = 0; i < 10; i++) {
      glm::vec3 position = cubePositions[i];
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, position);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      cube_shaderProgram.set4Matrix("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    cube.use();

    lighting_shaderProgram.use();
    lighting_shaderProgram.set4Matrix("view", view);
    lighting_shaderProgram.set4Matrix("projection", projection);
    cube.use();
    for (int i = 0; i < 4; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.2f));
      lighting_shaderProgram.set4Matrix("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    P_imgui::P_imgui_render();
    glfwSwapBuffers(window);
  }

  P_imgui::P_imgui_shutdown();
  glfwTerminate();
  return 0;
}
