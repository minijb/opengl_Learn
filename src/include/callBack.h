#pragma once
#include <config.h>

struct GLFW_UserPoint {
  bool turn_able = true;
  bool move_able = true;
  bool first_mouse = true;
};
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void project_init_callback(GLFWwindow *window);
void pressed_key(GLFWwindow *window);
