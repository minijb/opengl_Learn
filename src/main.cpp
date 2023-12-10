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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imguiT.h"

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
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

  // ========= build and compile our shader program =========
  Shader vertex("./shader/light/vertex.glsl", 0);
  Shader lighting("./shader/light/lighting_v.glsl", 0);
  Shader cube_fragment("./shader/light/fragment.glsl", 1);
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
  float *vertices = cube_vertices_normal();
  Cube cube(vertices, cube_state::with_normal);
  delete vertices;

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

      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                                     // and append into it.

      ImGui::Text("This is some useful text."); // Display some text (you can
                                                // use a format strings too)
      ImGui::Checkbox(
          "Demo Window",
          &show_demo_window); // Edit bools storing our window open/close state

      ImGui::SliderFloat("float", &f, 0.0f,
                         1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3(
          "clear color",
          (float *)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button")) // Buttons return true when clicked (most
                                   // widgets return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    // Rendering
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w,
                           &display_h); // 放置窗口拉伸导致的形变

    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);

    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cube_shaderProgram.use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::mat4(1.0f);
    projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)display_w / (float)display_h, 0.1f, 100.0f);
    cube_shaderProgram.set4Matrix("view", view);
    cube_shaderProgram.set4Matrix("projection", projection);
    cube_shaderProgram.set4Matrix("model", model);
    cube_shaderProgram.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    cube_shaderProgram.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    cube_shaderProgram.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    cube_shaderProgram.setFloat("material.shininess", 32.0f);
    cube_shaderProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    cube_shaderProgram.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    cube_shaderProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    cube_shaderProgram.setVec3("light.position", lightPos);
    cube_shaderProgram.setVec3("viewPos", camera.Position);
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

    P_imgui::P_imgui_render();
    glfwSwapBuffers(window);
  }

  P_imgui::P_imgui_shutdown();
  glfwTerminate();
  return 0;
}
