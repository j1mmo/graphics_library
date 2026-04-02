#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <vec3.hpp>

struct Light {
  vec3 _position;
  
  vec3 _ambient;
  vec3 _diffuse;
  vec3 _specular;
};

struct DirectionalLight {
  vec3 _direction;
  
  vec3 _ambient;
  vec3 _diffuse;
  vec3 _specular;
};

struct PointLight {
  vec3 _position;

  vec3 _ambient;
  vec3 _diffuse;
  vec3 _specular;

  float _constant;
  float _linear;
  float _quadratic;
};

struct FlashLight {
  vec3 _position;
  vec3 _direction;
  /*
  vec3 _ambient;
  vec3 _diffuse;
  vec3 _specular;
  */
  float _cutOff;
  float _outerCutOff;
};

#endif // LIGHT_HPP
