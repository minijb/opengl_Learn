#include "project_init.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <callBack.h>

extern GLFW_UserPoint glfw_userpoint;

void glfwProjectInit() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  stbi_set_flip_vertically_on_load(true);
}

GLFWwindow *windowCreate(int width, int height) {
  // create window
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(0);
  }
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, &glfw_userpoint);
  return window;
}

void gladInit(GLFWwindow *window, int width, int height) {
  // glad init
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(0);
  }
  // glad viewport
  glViewport(0, 0, width, height);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void global_OpenglState() { glEnable(GL_DEPTH_TEST); }
