#include "camera.h"
#include <GLFW/glfw3.h>
#include <callBack.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
extern float lastX;
extern float lastY;

extern Camera camera;

extern float deltaTime;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  lastX = static_cast<float>(width) / 2;
  lastY = static_cast<float>(height) / 2;
}

void processInput(GLFWwindow *window) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = 2.5f * deltaTime;
  GLFW_UserPoint *point = (GLFW_UserPoint *)glfwGetWindowUserPointer(window);
  if (point->move_able) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT, deltaTime);
  }

  // change mode
  pressed_key(window);
}

void pressed_key(GLFWwindow *window) {
  static bool pressed = false;

  GLFW_UserPoint *point = (GLFW_UserPoint *)glfwGetWindowUserPointer(window);
  if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS &&
      pressed == false) {

    if (point->move_able) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    point->turn_able = point->turn_able ? false : true;
    point->move_able = point->move_able ? false : true;
    point->first_mouse = true;
    pressed = true;
  } else if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE &&
             pressed == true) {
    pressed = false;
  }
}
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {

  GLFW_UserPoint *point = (GLFW_UserPoint *)glfwGetWindowUserPointer(window);
  if (point->turn_able) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (point->first_mouse) {
      lastX = xpos;
      lastY = ypos;
      point->first_mouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void project_init_callback(GLFWwindow *window) {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);
}
