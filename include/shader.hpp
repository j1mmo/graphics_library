#ifndef SHADER_HPP
#define SHADER_HPP
#pragma once

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "types.hpp"
#include <maths/vec3.hpp>
#include <maths/vec4.hpp>
#include <maths/mat4.hpp>

struct Shader {
  uint handle;

  void use() const {
    glUseProgram(handle);
  }

  void setBool(const char* name, bool value) const {
    int location = glGetUniformLocation(handle, name);
    if (location >= 0) {
	glUniform1i(location, (int)value);
    } else {
	spdlog::error("could not find {} in shader handle: {}", name, handle);
    }
  }

  void setInt(const char* name, bool value) const {
    int location = glGetUniformLocation(handle, name);
    if (location >= 0) {
	glUniform1i(location, (int)value);
    } else {
	spdlog::error("could not find {} in shader handle: {}", name, handle);
    }
  }
  
  void setFloat(const char* name, float value) const {
    int location = glGetUniformLocation(handle, name);
    if (location >= 0) {
	glUniform1f(location, value);
    } else {
	spdlog::error("could not find {} in shader handle: {}", name, handle);
    }
  }

  void setVec3(const char* name, float x, float y, float z) const {
    int location = glGetUniformLocation(handle, name);
    if (location >= 0) {
	glUniform3f(location, x, y, z);
    } else {
	spdlog::error("could not find {} in shader handle: {}", name, handle);
    }
  }

  void setVec3(const char* name, vec3 vec) const {
    int location = glGetUniformLocation(handle, name);
    if (location >= 0) {
	glUniform3f(location, vec[0], vec[1], vec[2]);
    } else {
	spdlog::error("could not find {} in shader handle: {}", name, handle);
    }
  }

  void setVec4(const char* name, float x, float y, float z, float w) const {
    int location = glGetUniformLocation(handle, name);
    if (location >= 0) {
	glUniform4f(location, x, y, z, w);
    } else {
	spdlog::error("could not find {} in shader handle: {}", name, handle);
    }
		
  }

  void setVec4(const char* name, vec4 vec) {
    int location = glGetUniformLocation(handle, name);
    if (location >= 0) {
	glUniform4f(location, vec[0], vec[1], vec[2], vec[3]);
    } else {
	spdlog::error("could not find {} in shader handle: {}", name, handle);
    }
  }


  void setMat4(const char* name, mat4 mat) {
    int location = glGetUniformLocation(handle, name);
    if (location >= 0) {
	glUniformMatrix4fv(location, 1, GL_FALSE, mat.data);
    } else {
	spdlog::error("could not find {} in shader handle: {}", name, handle);
    }
  }

  void setMvp(mat4 view, mat4 model, mat4 projection) {
    setMat4("view", view);
    setMat4("model", model);
    setMat4("projection", projection);
  }

  void release() {
    glDeleteProgram(handle);
  }
};

namespace shader {
  Shader compile(const char* vertexShaderLocation, const char* fragShaderLocation);
}



#endif //SHADER_HPP
