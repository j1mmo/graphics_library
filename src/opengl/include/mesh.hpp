#ifndef MASH_HPP
#define MASH_HPP

#include <glad/glad.h>
#include <darray.hpp>

#include "types.hpp"
#include "snake.hpp"

namespace mesh {

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
  
  void generate_vao(const mesh::Handles& handle);
  void generate_ebo_stream(const mesh::Handles& handle, const darray<u32>& indicies);
  void generate_vbo_stream(const mesh::Handles& handle, const darray<float>& data);
  void generate_ebo(const mesh::Handles& handle, const darray<u32>& indicies);
  void generate_vbo(const mesh::Handles& handle, const darray<float>& data);

  void generate_vertex_array_attributes(const mesh::Handles& mesh, const darray<float> data, const Attributes& attributes);
  void set_vertex_attributes(const Attributes& attributes);
  
  void bind_vao(const mesh::Handles& handle);
  void bind_vbo(const mesh::Handles& handle);
  void bind_ebo(const mesh::Handles& handle);
  
  void draw_element_array(const mesh::Handles& mesh);
  void draw_vertex_arrays(const mesh::Handles& mesh);
 
  void generate_square(mesh::Handles& mesh);
  void generate_tube(mesh::Handles& mesh, const snake::Player& player);
  void generate_bending_tube(mesh::Handles& mesh);
  void generate_outer(mesh::Handles& mesh);
  void generate_circle(mesh::Handles& mesh);
};

#endif // MASH_HPP
