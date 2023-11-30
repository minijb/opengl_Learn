#pragma once
#include <config.h>

void glfwProjectInit();
GLFWwindow *windowCreate(int width, int height);
void gladInit(GLFWwindow *window, int width, int height);
