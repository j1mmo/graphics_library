#include <window_manager.hpp>
#include <general.hpp>
#include <cstdio>
void Window::Init(GLFWwindow * window, ImGuiIO& ui)
{
  window_handle = window;
  imgui_handle = &ui;
  glfwGetWindowSize(window, &window_width, &window_height);
  printf("%d %d", window_width, window_height);
}

mat4 Window::Projection() const
{
  return mat4::setPerspective(maths::radians(FOV),
			      static_cast<f32>(window_width) /
			      static_cast<f32>(window_height),
			      NEAR,
			      FAR);
}

