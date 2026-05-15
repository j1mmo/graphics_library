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
#include <snake.hpp>
#include <mesh.hpp>

#include <obj_loader.hpp>
#include <init.hpp>
#include <mesh_manager.hpp>
#include <window_manager.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera{};

float lastX = 400, lastY = 300;

void processInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
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
  Window window = init::Init_Everything();
  glfwSetCursorPosCallback(window.window_handle, MouseCallback);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  Mesh::Mesh_Manager mesh_manager;
  mesh_manager.Init();
  
  unsigned int VAO[2];
  glGenVertexArrays(2, VAO);
  unsigned int VBO[1];
  glGenBuffers(1, VBO);

  Mesh::Handles cube = {
    .vao = VAO[0],
    .vbo = VBO[0],
    .ebo = 0
  };

  Mesh::Handles light_handle = {
    .vao = VAO[1],
    .vbo = VBO[0],
    .ebo = 0
  };

  Mesh::Attributes objAttribute {
    .strideLength = 8,
    .data = { 3, 3, 2 }
  };

  Mesh::Attributes lightAttribute {
    .strideLength = 8,
    .data = { 3 }
  };

  Mesh::Handles obj_cube = mesh_manager.Create_Handle();
  Load_Obj("resources/obj/cube.obj", obj_cube);

  Mesh::Handles obj_cylinder = mesh_manager.Create_Handle();
  Load_Obj("resources/obj/cylinder.obj", obj_cylinder);

  Mesh::Handles obj_half_bend = mesh_manager.Create_Handle();
  Load_Obj("resources/obj/half_bend.obj", obj_half_bend);

  Mesh::Handles obj_tail = mesh_manager.Create_Handle();
  Load_Obj("resources/obj/tail.obj", obj_tail);

  Mesh::bind_vao(cube);
  Mesh::generate_vbo(cube, vertices);
  Mesh::set_vertex_attributes(objAttribute);
  Mesh::bind_vao(light_handle);
  Mesh::bind_vbo(light_handle);
  Mesh::set_vertex_attributes(lightAttribute);

  Mesh::Handles square = mesh_manager.Create_Handle();
  Mesh::Handles outer = mesh_manager.Create_Handle();
  Mesh::generate_square(square);
  Mesh::generate_outer(outer);
    
  stbi_set_flip_vertically_on_load(true);
  Texture woodenBox = texture::load("resources/container2.png");
  Texture specularMap = texture::load("resources/container2_specular.png");

  Shader colour = shader::compile("shaders/lighting.vert", "shaders/lighting.frag");
  colour.use();
  colour.setInt("material.diffuse", 0);
  colour.setInt("material.specular", 1);
  Shader light  = shader::compile("shaders/light.vert", "shaders/light.frag");
  Shader basic  = shader::compile("shaders/basic.vert", "shaders/basic.frag");
  Shader snake  = shader::compile("shaders/snake.vert", "shaders/snake.frag");
  Shader snake_outline = shader::compile("shaders/snake.vert", "shaders/stencil_outline.frag");
  
  mat4 projection = window.Projection();

  DirectionalLight lightData = {
    ._direction = { 1.2f, 1.0f, 2.0f },
    ._ambient  = { 0.2f, 0.2f, 0.2f },
    ._diffuse  = { 0.5f, 0.5f, 0.5f },
    ._specular = { 1.0f, 1.0f, 1.0f }
  };

  Light snake_light = {
    ._position = { 1.2f, 1.0f, 2.0f },
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

  FlashLight flashLight = {
    ._position    = {camera._position},
    ._direction   = {camera._front},
    ._cutOff      = cosf(maths::radians(12.5f)),
    ._outerCutOff = cosf(maths::radians(17.5f))
  };

  float timer{0};
  while(!glfwWindowShouldClose(window.window_handle)) {

      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      timer += deltaTime;

      processInput(window.window_handle);

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
  
      if (timer >= 0.5f) {
	  bool growing = gameState.loop();
	  timer -= 0.5f;
      }
      
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      
      glStencilMask(0x00);
      mat4 view = camera.getView();
      


      light.use();
      
      mat4 model = mat4{};
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
      
      for (u32 x{0}; x < 8; x++) {
	  for (u32 z{0}; z < 8; z++) {
	      mat4 model;
	      vec3 position = {(float)x, 0, (float)z};
	      model = translate(model, position);
	      basic.setMat4("model", model);
	      basic.setVec3("colour", vec3{0.1, 0.2, 0.6});
	      Mesh::draw_element_array(square);
	      basic.setVec3("colour", vec3{0.1, 0.6, 0.3});
	      Mesh::draw_element_array(outer);
	  }
      }
      

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

      //render head
      model = translate(model, vec3{
	  (float)gameState.player_.head[0],
	  0,
	  gameState.player_.head[1]});
      colour.setMat4("model", model);
      glBindVertexArray(VAO[0]);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // render food
      model = mat4{};
      model = translate(model, vec3{
	  (float) gameState.food_[0],
	  0,
	  gameState.food_[1]});
      colour.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glStencilMask(0xFF);
      
      snake.use();
      model = mat4{};
      snake.setLight(snake_light);
      snake.setFloat("material.diffuse", 0.2);
      snake.setFloat("material.specular", 0.6);
      snake.setFloat("material.shininess", 0.8);
      snake.setMat4("view", view);
      snake.setMat4("projection", projection);
      // render snake body - messy? clean up?
      // Switch to wireframe mode
      for (u32 i{0}; i < gameState.player_.body_length_; i++) {
	  mat4 model = mat4{};
	  vec2 current = gameState.player_.body[i];
	  vec2 previous = (i == 0) ? gameState.player_.head : gameState.player_.body[i - 1];
	  vec2 next {-1.0f, -1.0f};
	  bool tail{false};
	  if (i == gameState.player_.body_length_ - 1) {
	      tail = true;
	  } else {
	      next = gameState.player_.body[i + 1];
	  }
	  vec2 direction_in = current - previous;
	  vec2 direction_out = next - current;

	  bool is_bend{true};
	  if (next[0] != -1.0f && next[1] != -1.0f) {
	      is_bend = (direction_in[0] != direction_out[0] ||
			 direction_in[1] != direction_out[1]);
	  }

	  direction_in.Absolute();
	  model = translate(model, vec3{(float) current[0], 0, current[1]}); 
	  snake.setVec3("colour", vec3{0.7, 0.3, 0.1});
	  if (tail == true) {
	      
	      vec2 direction = previous - current;
	      float turn_direction = vec2::dot_product(direction_in, current);
	      f32 rotation_angle{0.0f};
	      i32 x = static_cast<i32>(direction[0]);
	      i32 y = static_cast<i32>(direction[1]);
	      if (x == 1) {
		  rotation_angle = 90.0f;
	      } else if (x == -1) {
		  rotation_angle = 270.0f;
	      } else if (y == -1) {
		  rotation_angle = 0.0f;
	      } else if (y == 1) {
		  rotation_angle = 180.0f;
	      }
	      model = rotate(model, maths::radians(rotation_angle), vec3(0, 1, 0));
	      snake.setMat4("model", model);
	      Mesh::bind_vao(obj_tail);
	      Mesh::draw_vertex_arrays(obj_tail);
	  }
	  else if (true == is_bend) {
	      vec2 direction = previous - current;
	      float dot = vec2::dot_product(direction, direction_out);
	      float rotation{0.0f};
	      if (dot == 0.0f) {
		  float turn_direction = (direction_in[0] * direction_out[1]) - (direction_in[1] * direction_out[0]);
		  vec2 sum = direction + direction_out;
		  i32 x = static_cast<i32>(sum[0]);
		  i32 y = static_cast<i32>(sum[1]);
		  ImGui::Text("direction: %f\n %d %d", turn_direction, x, y);
		  if (turn_direction > 0.0f) {
		      if (x == 1 && y == 1) {
			  rotation = 0.0f;
		      } else if (x == -1 && y ==  1) {
			  rotation = 90.0f;
		      } else if (x == -1 && y == -1) {
			  rotation = 180.0f;
		      } else if (x ==  1 && y == -1) {
			  rotation = 270.0f;
		      }
		  } else {
		      if (x ==  1 && y ==  1) {
			  rotation = 0.0f;
		      } else if (x == -1 && y ==  1) {
			  rotation = 90.0f;
		      } else if (x == -1 && y == -1) {
			  rotation = 180.0f;
		      } else if (x ==  1 && y == -1) {
			  rotation = 270.0f;
		      }
		  }
	      }
	      
	      model = rotate(model, maths::radians(rotation), vec3(0, 1, 0));
	      snake.setMat4("model", model);
	      Mesh::bind_vao(obj_half_bend);
	      Mesh::draw_vertex_arrays(obj_half_bend);
	  }
	  else {    
	      vec2 direction = previous - current;
	      model = rotate(model, maths::radians(90.0f), vec3(0, direction[0], direction[1]));
	      snake.setMat4("model", model);
	      Mesh::bind_vao(obj_cylinder);
	      Mesh::draw_vertex_arrays(obj_cylinder);
	  }
      }
      
      // second pass
      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      glStencilMask(0x00);
      glDisable(GL_DEPTH_TEST);
      snake_outline.use();
      snake_outline.setMat4("view", view);
      snake_outline.setMat4("projection", projection);
      for (u32 i{0}; i < gameState.player_.body_length_; i++) {
	  f32 scale_factor = 1.5f;
	  mat4 model = mat4{};
	  vec2 current = gameState.player_.body[i];
	  vec2 previous = (i == 0) ? gameState.player_.head : gameState.player_.body[i - 1];
	  vec2 next {-1.0f, -1.0f};
	  bool tail{false};
	  if (i == gameState.player_.body_length_ - 1) {
	      tail = true;
	  } else {
	      next = gameState.player_.body[i + 1];
	  }
	  vec2 direction_in = current - previous;
	  vec2 direction_out = next - current;

	  bool is_bend{true};
	  if (next[0] != -1.0f && next[1] != -1.0f) {
	      is_bend = (direction_in[0] != direction_out[0] ||
			 direction_in[1] != direction_out[1]);
	  }

	  direction_in.Absolute();
	  model = translate(model, vec3{(float) current[0], 0, current[1]}); 
	  if (tail == true) {
	      
	      vec2 direction = previous - current;
	      float turn_direction = vec2::dot_product(direction_in, current);
	      f32 rotation_angle{0.0f};
	      i32 x = static_cast<i32>(direction[0]);
	      i32 y = static_cast<i32>(direction[1]);
	      if (x == 1) {
		  rotation_angle = 90.0f;
	      } else if (x == -1) {
		  rotation_angle = 270.0f;
	      } else if (y == -1) {
		  rotation_angle = 0.0f;
	      } else if (y == 1) {
		  rotation_angle = 180.0f;
	      }
	      model = scale(model, vec3{scale_factor, scale_factor, scale_factor});
	      model = rotate(model, maths::radians(rotation_angle), vec3(0, 1, 0));
	      snake_outline.setMat4("model", model);
	      Mesh::bind_vao(obj_tail);
	      Mesh::draw_vertex_arrays(obj_tail);
	  }
	  else if (true == is_bend) {
	      vec2 direction = previous - current;
	      float dot = vec2::dot_product(direction, direction_out);
	      float rotation{0.0f};
	      if (dot == 0.0f) {
		  float turn_direction = (direction_in[0] * direction_out[1]) - (direction_in[1] * direction_out[0]);
		  vec2 sum = direction + direction_out;
		  i32 x = static_cast<i32>(sum[0]);
		  i32 y = static_cast<i32>(sum[1]);
		  ImGui::Text("direction: %f\n %d %d", turn_direction, x, y);
		  if (turn_direction > 0.0f) {
		      if (x == 1 && y == 1) {
			  rotation = 0.0f;
		      } else if (x == -1 && y ==  1) {
			  rotation = 90.0f;
		      } else if (x == -1 && y == -1) {
			  rotation = 180.0f;
		      } else if (x ==  1 && y == -1) {
			  rotation = 270.0f;
		      }
		  } else {
		      if (x ==  1 && y ==  1) {
			  rotation = 0.0f;
		      } else if (x == -1 && y ==  1) {
			  rotation = 90.0f;
		      } else if (x == -1 && y == -1) {
			  rotation = 180.0f;
		      } else if (x ==  1 && y == -1) {
			  rotation = 270.0f;
		      }
		  }
	      }
	      model = scale(model, vec3{scale_factor, scale_factor, scale_factor});
	      model = rotate(model, maths::radians(rotation), vec3(0, 1, 0));
	      snake_outline.setMat4("model", model);
	      Mesh::bind_vao(obj_half_bend);
	      Mesh::draw_vertex_arrays(obj_half_bend);
	  }
	  else {    
	      vec2 direction = previous - current;
	      model = scale(model, vec3{scale_factor, scale_factor, scale_factor});
	      model = rotate(model, maths::radians(90.0f), vec3(0, direction[0], direction[1]));
	      snake_outline.setMat4("model", model);
	      Mesh::bind_vao(obj_cylinder);
	      Mesh::draw_vertex_arrays(obj_cylinder);
	  }
      }
      

      glStencilMask(0xFF);
      glStencilFunc(GL_ALWAYS, 0, 0xFF);
      glEnable(GL_DEPTH_TEST);

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
      glfwSwapBuffers(window.window_handle);
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
      state = !state;
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


