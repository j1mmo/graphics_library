#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <shader.hpp>
#include <texture.hpp>
#include <types.hpp>
#include <array.hpp>
#include <mat4.hpp>
#include <vec4.hpp>
#include <general.hpp>
#include <vertex_array.hpp>
#include <matrix_transformations.hpp>
#include <camera.hpp>
#include <snake.hpp>
#include <mesh.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window);
void processKeyboard(GLFWwindow* window, double xpos, double ypos);

Camera camera{};

float lastX = 400, lastY = 300;

snake::game_state gameState;

darray<float> vertices = {
  // positions          // normals           // texture coords
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

i32 getMaximumVertexAttributes() {
  i32 nrAttributes{0};
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  return nrAttributes;
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  glfwSetErrorCallback([](int error, const char* description) {
    spdlog::error("GLFW Error ({}): {}", error, description);
  });
  
  u32 width = 1920, height = 1080;
  GLFWwindow * window = glfwCreateWindow(width, height, "Window", NULL, NULL);
  if (window == nullptr) {
      spdlog::critical("Failed to create GLFW Window.");
      glfwTerminate();
      return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      spdlog::critical("Failed to initialise GLAD.");
      return -1;
  }

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  glfwSetCursorPosCallback(window, processKeyboard);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // imgui init
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
  
  glEnable(GL_DEPTH_TEST);

  unsigned int VAO[2];
  glGenVertexArrays(2, VAO);
  unsigned int VBO[1];
  glGenBuffers(1, VBO);

  mesh::Handles cube = {
    .vao = VAO[0],
    .vbo = VBO[0],
    .ebo = 0
  };

  mesh::Handles light_handle = {
    .vao = VAO[1],
    .vbo = VBO[0],
    .ebo = 0
  };

  mesh::Attributes objAttribute {
    .strideLength = 8,
    .data = { 3, 3, 2 }
  };

  mesh::Attributes lightAttribute {
    .strideLength = 8,
    .data = { 3 }
  };

  mesh::bind_vao(cube);
  mesh::generate_vbo(cube, vertices);
  mesh::set_vertex_attributes(objAttribute);
  mesh::bind_vao(light_handle);
  mesh::bind_vbo(light_handle);
  mesh::set_vertex_attributes(lightAttribute);

  u32 circle_vao;
  u32 circle_vbo;
  u32 circle_ebo;
  glGenVertexArrays(1, &circle_vao);
  glGenBuffers(1, &circle_vbo);
  glGenBuffers(1, &circle_ebo);
  mesh::Handles circle = {
    .vao = circle_vao,
    .vbo = circle_vbo,
    .ebo = circle_ebo,
    .drawElementsCount = 0
  };

  u32 tube_vao;
  u32 tube_vbo;
  u32 tube_ebo;
  glGenVertexArrays(1, &tube_vao);
  glGenBuffers(1, &tube_vbo);
  glGenBuffers(1, &tube_ebo);
  mesh::Handles tube = {
    .vao = tube_vao,
    .vbo = tube_vbo,
    .ebo = tube_ebo,
    .drawElementsCount = 0
  };

  unsigned int vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  mesh::Handles square = {
    .vao = vao,
    .vbo = vbo,
    .ebo = ebo,
    .drawElementsCount = 0
  };
  
  unsigned int vao1, vbo1, ebo1;
  glGenVertexArrays(1, &vao1);
  glGenBuffers(1, &vbo1);
  glGenBuffers(1, &ebo1);
  mesh::Handles outer = {
    .vao = vao1,
    .vbo = vbo1,
    .ebo = ebo1,
    .drawElementsCount = 0
  };
  
  mesh::generate_square(square);
  mesh::generate_outer(outer);
  mesh::generate_circle(circle);
  mesh::generate_tube(tube, gameState.player_);
    
  stbi_set_flip_vertically_on_load(true);
  Texture woodenBox = texture::load("resources/container2.png");
  Texture specularMap = texture::load("resources/container2_specular.png");

  Shader colour = shader::compile("shaders/lighting.vert", "shaders/lighting.frag");
  Shader light  = shader::compile("shaders/light.vert", "shaders/light.frag");
  Shader basic  = shader::compile("shaders/basic.vert", "shaders/basic.frag");
				 
  colour.use();
  colour.setInt("material.diffuse", 0);
  colour.setInt("material.specular", 1);
  
  mat4 projection = mat4::setPerspective(maths::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);
  
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  DirectionalLight lightData = {
    ._direction = { 1.2f, 1.0f, 2.0f },
    ._ambient  = { 0.2f, 0.2f, 0.2f },
    ._diffuse  = { 0.5f, 0.5f, 0.5f },
    ._specular = { 1.0f, 1.0f, 1.0f }
  };
  
  PointLight pointLight = {
      ._position = { 1.2f, 1.0f, 2.0f },
      ._ambient  = { 0.2f, 0.2f, 0.2f },
      ._diffuse  = { 0.5f, 0.5f, 0.5f },
      ._specular = { 1.0f, 1.0f, 1.0f },

      ._constant  = 1.0f,
      ._linear    = 0.09f,
      ._quadratic = 0.032f
  };

  Material material = {
    ._shininess = { 32.0f },
    ._diffuse   = { 1.0f, 0.5f, 0.31f}
  };

  array<vec3, 4> pointLightPositions {
    { 0.7f,  0.2f,  2.0f},
    { 2.3f, -3.3f, -4.0f},
    {-4.0f,  2.0f, -12.0f},
    { 0.0f,  0.0f, -3.0f}
  };

  float timer{0};
  while(!glfwWindowShouldClose(window)) {

      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      timer += deltaTime;

      processInput(window);

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      ImGui::Text("player position: %f %f %d", gameState.player_.head[0], gameState.player_.head[1], gameState.player_.body_length_);
      ImGui::Text("food position: %f %f\n", gameState.food_[0], gameState.food_[1]);
      
      if (timer >= 0.5f) {
	  bool growing = gameState.loop();
	  timer -= 0.5f;
      }
      
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      mat4 view = camera.getView();

      FlashLight flashLight = {
	._position    = {camera._position},
	._direction   = {camera._front},
	._cutOff      = cosf(maths::radians(12.5f)),
	._outerCutOff = cosf(maths::radians(17.5f))
      };
      
      colour.use();
      
      colour.setLight(pointLight, pointLightPositions);
      colour.setVec3("viewPos", camera._position);
      colour.setMaterial(material);

      glActiveTexture(GL_TEXTURE0);
      woodenBox.bind();
      glActiveTexture(GL_TEXTURE1);
      specularMap.bind();

      colour.setMat4("view", view);
      colour.setMat4("projection", projection);

      glBindVertexArray(VAO[0]);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      // render body
      for (u32 i{0}; i < gameState.player_.body_length_; i++) {
	  mat4 model;
	  vec2 v = gameState.player_.body[i];
	  model = translate(model, vec3{(float) v[0], 0, v[1]});
	  colour.setMat4("model", model);
	  glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      

      //render head
      mat4 model;
      model = translate(model, vec3{(float) gameState.player_.head[0], 0, gameState.player_.head[1]});
      colour.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // render food
      model = mat4{};
      model = translate(model, vec3{(float) gameState.food_[0], 0, gameState.food_[1]});
      colour.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      light.use();

      model = mat4{};
      model = translate(model, lightData._direction);
      model = scale(model, vec3{0.2f, 0.2f, 0.2f});
      light.setMat4("view", view);
      light.setMat4("model", model);
      light.setMat4("projection", projection);

      glBindVertexArray(VAO[1]);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      basic.use();
      model = mat4{};
      basic.setMat4("view", view);
      basic.setMat4("projection", projection);
      /*
      for (u32 x{0}; x < 8; x++) {
	  for (u32 z{0}; z < 8; z++) {
	      mat4 model;
	      vec3 position = {(float)x, 0 , (float)z};
	      model = translate(model, position);
	      basic.setMat4("model", model);
	      basic.setVec3("colour", vec3{0.3, 0.2, 0.8});
	      mesh::draw_element_array(square);
	      basic.setVec3("colour", vec3{0.8, 0.0, 0.1});
	      mesh::draw_element_array(outer);
	  }
      }

      mesh::bind_vao(circle);
      for (u32 x{0}; x < 8; x++) {
	  for (u32 z{0}; z < 8; z++) {
	      mat4 model;
	      vec3 position = {(float)x, 1.0 , (float)z};
	      model = translate(model, position);
	      basic.setMat4("model", model);
	      basic.setVec3("colour", vec3{0.8, 0.6, 0.1});
	      mesh::draw_element_array(circle);
	  }
      }
      */
      
      
      mesh::bind_vao(tube);
      model = mat4{};
      model = scale(model, {5, 5, 5});
      //model = rotate(model, maths::radians(deltaTime), vec3(0.0, 0.0, 1.0));
      basic.setMat4("model", model);
      basic.setVec3("colour", vec3{0.2, 0.6, 0.1});
      mesh::draw_element_array(tube);

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
  }

  static bool state = false;

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      state = true;
  }
    
  if (state) {
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(Camera::movement::forward, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(Camera::movement::backwards, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(Camera::movement::left, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(Camera::movement::right, deltaTime);
  } else {
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	gameState.player_.direction = snake::Direction::UP;
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	gameState.player_.direction = snake::Direction::DOWN;
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	gameState.player_.direction = snake::Direction::LEFT;
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	gameState.player_.direction = snake::Direction::RIGHT;
  }
}

void processKeyboard(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    
    if (firstMouse) {
	lastX = xpos;
	lastY = ypos;
	firstMouse = false;
    }
    
    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.processMouseMovement(xOffset, yOffset);
}
