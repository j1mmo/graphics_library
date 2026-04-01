#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "types.hpp"

#include <vec3.hpp>

struct Material {
  f32 _shininess;
  vec3 _ambient;
  vec3 _diffuse;
  vec3 _specular;

};

#endif // MATERIAL_HPP
