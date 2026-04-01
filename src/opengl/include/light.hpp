#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <vec3.hpp>

struct Light {
  vec3 _position;
  vec3 _ambient;
  vec3 _diffuse;
  vec3 _specular;
};

#endif // LIGHT_HPP
