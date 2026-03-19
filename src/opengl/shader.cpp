#include "shader.hpp"

#include <cstdio>
#include <cstdlib>

bool ShaderCompilationStatus(i32 shader, const char* shaderFileName = nullptr);
bool linkingStatus(i32 program);
int loadProgram(int type, const char* filename);

namespace shader {
 
  Shader compile(const char* vertexShaderLocation, const char* fragShaderLocation) {
    u32 vertexShader = loadProgram(GL_VERTEX_SHADER, vertexShaderLocation);
    u32 fragmentShader = loadProgram(GL_FRAGMENT_SHADER, fragShaderLocation);

    u32 shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    bool linkShaderStatus = linkingStatus(shaderProgram);

    if (false == linkShaderStatus) {
	glDeleteProgram(shaderProgram);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return Shader{shaderProgram};
  }

}

int loadProgram(int type, const char* filename) {
  i32 shader{0};
  FILE* file = nullptr;
  file = fopen(filename, "rb"); // Use fopen on Linux
  if (file) {
      // Get file length (using ftell in Linux)
      fseek(file, 0, SEEK_END);
      i32 len = ftell(file);
      fseek(file, 0, SEEK_SET); // Reset file pointer to the beginning

      char* data = (char*) malloc(len + 1);
      if (!data) {
	  fclose(file);
	  return 0;
      }

      // Read the file data
      i32 read{0}, pos{0};
      while ((read = fread(data + pos, 1, len - pos, file)) > 0){
	  pos += read;
      }
      fclose(file);
      // Ensure the data is null-terminated
      data[len] = '\0';
	  
      shader = glCreateShader(type);
      glShaderSource(shader, 1, &data, &len);
      glCompileShader(shader);
      free(data);

      bool compileShader = ShaderCompilationStatus(shader, filename);
      if (compileShader == false) {
	  glDeleteShader(shader);
      }
  }
  else {
      spdlog::error("Cannot open shader file: {}", filename);
  }
  
  return shader;
}

bool ShaderCompilationStatus(i32 shader, const char* shaderFileName)
{
  i32 status{0}, len{0};
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (false == status) {
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
      char* error = (char*) malloc(len + 1);
      glGetShaderInfoLog(shader, len, &status, error);
      error[len] = '\0';
      spdlog::error("unable to compile shader: {}", error);
      free(error);
      return false;
  }
  
  return true;
}


bool linkingStatus(i32 program)
{
  int status{0};
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == false) {
      int length{0};

      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
      char* error=(char*) malloc(length+1);
      glGetProgramInfoLog(program, length, &status, error);
      error[length]='\0';
      spdlog::error("failed to link shaders: {}", error);
      free(error);
      return false;
  }
  return true;
}




