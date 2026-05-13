#ifndef INIT_HPP
#define INIT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "types.hpp"

namespace init {
  constexpr u32 SCREEN_WIDTH{1920};
  constexpr u32 SCREEN_HEIGHT{1080};
  constexpr u32 GL_VERSION_MAJOR{3};
  constexpr u32 GL_VERSION_MINOR{3};
  
  bool glfw();
  [[nodiscard]] GLFWwindow* create_window();
  bool Glad();
  void Set_Callback_Functions(GLFWwindow* window);
  ImGuiIO& Imgui(GLFWwindow* window);
}

#endif // INIT_HPP
