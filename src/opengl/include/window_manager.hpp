#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include "types.hpp"
#include <mat4.hpp>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

struct Window {
  static constexpr f32 FOV{45.0f};
  static constexpr f32 NEAR{0.1f};
  static constexpr f32 FAR{100.0f};
  GLFWwindow* window_handle{nullptr};
  i32 window_width{0};
  i32 window_height{0};
  ImGuiIO* imgui_handle{nullptr};

  void Init(GLFWwindow * window, ImGuiIO& io);
  mat4 Projection() const;
};

#endif // WINDOW_MANAGER_HPP
