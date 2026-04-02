#ifndef SHADER_HPP
#define SHADER_HPP
#pragma once

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "types.hpp"
#include "light.hpp"
#include "material.hpp"

#include <array.hpp>

#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4.hpp>

struct Shader {
  u32 handle;

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
	glUniformMatrix4fv(location, 1, GL_TRUE, mat.data);
    } else {
	spdlog::error("could not find {} in shader handle: {}", name, handle);
    }
  }

  void setMvp(mat4 view, mat4 model, mat4 projection) {
    setMat4("view", view);
    setMat4("model", model);
    setMat4("projection", projection);
  }
  
  //needs updating perhaps for difference variable names - also material
  void setLight(const Light& light) const {
    setVec3("light.position", light._position);
    setVec3("light.ambient",  light._ambient);
    setVec3("light.diffuse",  light._diffuse);
    setVec3("light.specular", light._specular); 
  }

  void setLight(const DirectionalLight& light) const {
    setVec3("light.direction", light._direction);
    setVec3("light.ambient",  light._ambient);
    setVec3("light.diffuse",  light._diffuse);
    setVec3("light.specular", light._specular);
  }

  void setLight(const PointLight& light) const {
    setVec3("light.position", light._position);
    setVec3("light.ambient",  light._ambient);
    setVec3("light.diffuse",  light._diffuse);
    setVec3("light.specular", light._specular);
    setFloat("light.constant", light._constant);
    setFloat("light.linear", light._linear);
    setFloat("light.quadratic", light._quadratic);
  }

  void setLight(const PointLight& light, const array<vec3, 4>& positions) const {
    setVec3("pointLights[0].position", positions[0]);
    setVec3("pointLights[0].ambient",  light._ambient);
    setVec3("pointLights[0].diffuse",  light._diffuse);
    setVec3("pointLights[0].specular", light._specular);
    setFloat("pointLights[0].constant", light._constant);
    setFloat("pointLights[0].linear", light._linear);
    setFloat("pointLights[0].quadratic", light._quadratic);

    setVec3("pointLights[1].position", positions[1]);
    setVec3("pointLights[1].ambient",  light._ambient);
    setVec3("pointLights[1].diffuse",  light._diffuse);
    setVec3("pointLights[1].specular", light._specular);
    setFloat("pointLights[1].constant", light._constant);
    setFloat("pointLights[1].linear", light._linear);
    setFloat("pointLights[1].quadratic", light._quadratic);

    setVec3("pointLights[2].position", positions[2]);
    setVec3("pointLights[2].ambient",  light._ambient);
    setVec3("pointLights[2].diffuse",  light._diffuse);
    setVec3("pointLights[2].specular", light._specular);
    setFloat("pointLights[2].constant", light._constant);
    setFloat("pointLights[2].linear", light._linear);
    setFloat("pointLights[2].quadratic", light._quadratic);

    setVec3("pointLights[3].position", positions[3]);
    setVec3("pointLights[3].ambient",  light._ambient);
    setVec3("pointLights[3].diffuse",  light._diffuse);
    setVec3("pointLights[3].specular", light._specular);
    setFloat("pointLights[3].constant", light._constant);
    setFloat("pointLights[3].linear", light._linear);
    setFloat("pointLights[3].quadratic", light._quadratic);
  }

  void setLight(const FlashLight& light) const {
    setVec3("spotLight.position", light._position);
    setVec3("spotLight.direction", light._direction);
    setFloat("spotLight.cutOff", light._cutOff);
    setFloat("spotLight.outerCutOff", light._outerCutOff);
  }

  void setMaterial(const Material& material) const {
    setFloat("material.shininess", material._shininess);
    setVec3("material.diffuse",  material._diffuse);
  }

  void release() {
    glDeleteProgram(handle);
  }
};

namespace shader {
  Shader compile(const char* vertexShaderLocation, const char* fragShaderLocation);
}



#endif //SHADER_HPP
