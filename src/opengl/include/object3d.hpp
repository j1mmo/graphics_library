#ifndef OBJ_OBJECT_HPP
#define OBJ_OBJECT_HPP

#include <darray.hpp>
#include <vec3.hpp>
#include <vec2.hpp>

namespace object3d {
  
  struct data {
    darray<vec3> _vertices;
    darray<vec3> _normals;
    darray<vec2> _texCoords;
    darray<vec3> _faces;
  };
  
  data loadObject3d(const char* filename);
}

#endif // OBJ_OBJECT_HPP
