#include <init.hpp>

#include <spdlog/spdlog.h>

bool init::glfw()
{
  bool glfw_init_state = glfwInit();

  if (glfw_init_state == GLFW_FALSE) {
      spdlog::error("Unable to initialise glfw: function glfwInit()");
      return false;
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, init::GL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, init::GL_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  return true;
}

GLFWwindow* init::create_window()
{
  GLFWwindow * window =
    glfwCreateWindow(
		     init::SCREEN_WIDTH,
		     init::SCREEN_HEIGHT,
		     "Snake",
		     nullptr,
		     nullptr);
  if (nullptr == window) {
      spdlog::critical("Failed to create GLFW Window.");
      glfwTerminate();
      return nullptr;
  }
  glfwMakeContextCurrent(window);
  return window;
}

bool init::Glad()
{
  if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      spdlog::critical("Failed to initialise GLAD.");
      return false;
  }
  return true;
}

void init::Set_Callback_Functions(GLFWwindow * window)
{
  glfwSetErrorCallback([](int error, const char* description) {
    spdlog::error("GLFW Error ({}): {}", error, description);
  });
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });
}

ImGuiIO& init::Imgui(GLFWwindow* window)
{
  bool version = IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  return io;
}
