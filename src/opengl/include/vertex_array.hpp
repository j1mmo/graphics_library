#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <glad/glad.h>

#include "types.hpp"

namespace vertex {
  
  // structs don't have ownership of the data
  struct Array {
    u32 vao{0};
    u32 vbo{0};
  };

  struct Element {
    u32 vao{0};
    u32 vbo{0};
    u32 ebo{0};
  };

  void generate(const Array& array, u32 dataSize, float* dataPointer, u32* attributeSize, u32 attribSizeCount);
}

#endif // VERTEX_ARRAY_HPP
