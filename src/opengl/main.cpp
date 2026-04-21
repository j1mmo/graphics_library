#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

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

#include <mesh.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window);
void processKeyboard(GLFWwindow* window, double xpos, double ypos);

Camera camera{};

float lastX = 400, lastY = 300;

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
/*
#include <object3d.hpp>

int main() {
  object3d::data d =object3d::loadObject3d("resources/obj/pyramid.obj");
  
}
*/
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
  stbi_set_flip_vertically_on_load(true);
  glEnable(GL_DEPTH_TEST);

  unsigned int VAO[2];
  glGenVertexArrays(2, VAO);

  unsigned int VBO[1];
  glGenBuffers(1, VBO);

  Mesh::Attributes objAttribute {
    .strideLength = 8,
    .data = { 3, 3, 2 }
  };

  Mesh::Attributes lightAttribute {
    .strideLength = 8,
    .data = { 3 }
  };
  
  Mesh::bindVAO(VAO[0]);
  Mesh::bindVBO(VBO[0], vertices);
  Mesh::setVertexAttributes(VBO[0], objAttribute);
  Mesh::bindVAO(VAO[1]);
  Mesh::bindVBO(VBO[0]);
  Mesh::setVertexAttributes(VBO[1], lightAttribute);

  Texture woodenBox = texture::load("resources/container2.png");
  Texture specularMap = texture::load("resources/container2_specular.png");

  Shader colour = shader::compile("shaders/lighting.vert", "shaders/lighting.frag");
  Shader light    = shader::compile("shaders/light.vert", "shaders/light.frag");

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

  array<vec3, 10> cubePositions = {
    { 0.0f,  0.0f,  0.0f},
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f},
    { 2.4f, -0.4f, -3.5f},
    {-1.7f,  3.0f, -7.5f},
    { 1.3f, -2.0f, -2.5f},
    { 1.5f,  2.0f, -2.5f},
    { 1.5f,  0.2f, -1.5f},
    {-1.3f,  1.0f, -1.5f}
  };
  
  while(!glfwWindowShouldClose(window)) {

      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(window);
      
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

      for (u32 i{0}; i < 10; i++) {
	  mat4 model;
	  model = translate(model, cubePositions[i]);
	  float angle = 20.0f * i;
	  model = rotate(model, maths::radians(angle), vec3{1.0f, 0.3f, 0.5});
	  colour.setMat4("model", model);

	  glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      
      mat4 model;
      light.use();

      model = mat4{};
      model = translate(model, lightData._direction);
      model = scale(model, vec3{0.2f, 0.2f, 0.2f});
      light.setMat4("view", view);
      light.setMat4("model", model);
      light.setMat4("projection", projection);

      glBindVertexArray(VAO[1]);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      
      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.processKeyboard(Camera::movement::forward, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboard(Camera::movement::backwards, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboard(Camera::movement::left, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboard(Camera::movement::right, deltaTime);
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
