#ifndef MASH_HPP
#define MASH_HPP

#include <glad/glad.h>
#include <darray.hpp>

#include "types.hpp"
#include "snake.hpp"

namespace Mesh {

  struct Attributes {
    u32 strideLength;
    darray<u32> data;
  };

  struct Handles {
    u32 vao{0};
    u32 vbo{0};
    u32 ebo{0};
    u32 drawElementsCount{0};
  };
  
  void generate_vao(const Mesh::Handles& handle);
  void generate_ebo_stream(const Mesh::Handles& handle, const darray<u32>& indicies);
  void generate_vbo_stream(const Mesh::Handles& handle, const darray<float>& data);
  void generate_ebo(const Mesh::Handles& handle, const darray<u32>& indicies);
  void generate_vbo(const Mesh::Handles& handle, const darray<float>& data);

  void generate_vertex_array_attributes(const Mesh::Handles& Mesh, const darray<float> data, const Attributes& attributes);
  void set_vertex_attributes(const Attributes& attributes);
  
  void bind_vao(const Mesh::Handles& handle);
  void bind_vbo(const Mesh::Handles& handle);
  void bind_ebo(const Mesh::Handles& handle);
  
  void draw_element_array(const Mesh::Handles& Mesh);
  void draw_vertex_arrays(const Mesh::Handles& Mesh);

  void Generate_Cone(Mesh::Handles& Mesh);
  void generate_square(Mesh::Handles& Mesh);
  void generate_tube(Mesh::Handles& Mesh, const snake::Player& player);
  void generate_bending_tube(Mesh::Handles& Mesh);
  void generate_outer(Mesh::Handles& Mesh);
  void generate_circle(Mesh::Handles& Mesh);
};

#endif // MASH_HPP
