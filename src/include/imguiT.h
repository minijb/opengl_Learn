#pragma once
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "iostream"

namespace P_imgui {

void P_imguiInit(GLFWwindow *window);
void P_Loop_init();

void P_imgui_shutdown();
void P_imgui_render();

} // namespace P_imgui
