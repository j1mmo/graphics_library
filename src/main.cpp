#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "shader.hpp"
#include "texture.hpp"
#include "types.hpp"

#include <maths/mat4.hpp>
#include <maths/vec4.hpp>
#include <maths/matrix_transformations.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
  }
}

mat4 mat;

float firstTriangle[] = {
  -0.9f, -0.5f, 0.0f,  // left 
  -0.0f, -0.5f, 0.0f,  // right
  -0.45f, 0.5f, 0.0f,  // top 
};

float secondTriangle[] = {
  0.0f, -0.5f, 0.0f,  // left
  0.9f, -0.5f, 0.0f,  // right
  0.45f, 0.5f, 0.0f   // top 
};

float vertices[] = {
  // positions          // colors           // texture coords
  0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
  0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
  -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

u32 indicies[] = {
  0, 1, 3,
  1, 2, 3
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
  
  GLFWwindow * window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
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

  stbi_set_flip_vertically_on_load(true);

  unsigned int VAO[3];
  glGenVertexArrays(3, VAO);
  glBindVertexArray(VAO[0]);

  unsigned int VBO[3];
  glGenBuffers(3, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO[1]);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO[2]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  u32 EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  Texture texture = texture::load("resources/container.jpg");
  Texture texture2 = texture::load("resources/awesomeface.png");

  Shader shader = shader::compile("shaders/basic.vert", "shaders/basic.frag");
  Shader yellowShader = shader::compile("shaders/basic.vert", "shaders/basic_2.frag");
  Shader multiShader = shader::compile("shaders/position_colour.vert", "shaders/position_colour.frag");

  vec4 vec{1.0f, 0.0f, 0.0f, 1.0f};
  mat4 trans{};
  trans = translate(trans, vec3{1.0f, 1.0f, 0.0f});
  vec = trans * vec;
  spdlog::error("{} {} {}", vec[0], vec[1], vec[2]);

  multiShader.use();
  multiShader.setInt("texture2", 1);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
  while(!glfwWindowShouldClose(window)) {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      processInput(window);
      
      shader.use();
      glBindVertexArray(VAO[0]);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      yellowShader.use();
      glBindVertexArray(VAO[1]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      multiShader.use();
      glActiveTexture(GL_TEXTURE0);
      texture.bind();
      glActiveTexture(GL_TEXTURE1);
      texture2.bind();
      glBindVertexArray(VAO[2]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   
      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
