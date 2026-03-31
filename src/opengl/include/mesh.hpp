#ifndef MASH_HPP
#define MASH_HPP

#include <glad/glad.h>

#include "types.hpp"

#include <array.hpp>

struct Mesh {

  struct Attributes {
    u32 strideLength;
    array<u32, 2> data;
  };
  u32 vao{0}, vbo{0}, ebo{0};


  template<u32 dataSize> static
  void generate(const Mesh& mesh, const array<float, dataSize>& data, const Attributes& attributes) {
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data._data, GL_STATIC_DRAW);

    i32 offsetCounter{0}, index{0};
    for (const u32 attributeSize : attributes.data) {
	glVertexAttribPointer(index, attributeSize, GL_FLOAT, GL_FALSE, attributes.strideLength * sizeof(float), (void*) (offsetCounter * sizeof(float)));
	glEnableVertexAttribArray(index);
	offsetCounter += attributeSize;
	index++;
    }

    glEnableVertexAttribArray(index);
  }

  template<u32 dataSize> static
  void bindVBO(const u32 vbo, const array<float, dataSize>& data) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data._data, GL_STATIC_DRAW);
  }

  template<u32 dataSize> static
  void setVertexAttribues(const u32 vbo, const Attributes& attributes) {
    i32 offsetCounter{0}, index{0};
    
    for (const u32 attributeSize : attributes.data) {
	glVertexAttribPointer(index, attributeSize, GL_FLOAT, GL_FALSE, attributes.strideLength * sizeof(float), (void*) (offsetCounter * sizeof(float)));
	glEnableVertexAttribArray(index);
	offsetCounter += attributeSize;
	index++;
    }
  }
};

#endif // MASH_HPP
