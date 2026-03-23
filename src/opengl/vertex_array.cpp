#include <vertex_array.hpp>

namespace vertex
{
  void generate(const Array& array, u32 dataSize, float* dataPointer, u32* attributeSize, u32 attribSizeCount)
  {
    glBindVertexArray(array.vao);
    glBindBuffer(GL_ARRAY_BUFFER, array.vbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, dataPointer, GL_STATIC_DRAW);

    i32 offsetCounter{0};
    for (u32 attribIndex{0}; attribIndex < attribSizeCount; attribIndex++) {
	u32 size = attributeSize[attribSizeCount];
	glVertexAttribPointer(attribIndex, *attributeSize, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*) (offsetCounter * sizeof(float)));
	glEnableVertexAttribArray(attribIndex);
	offsetCounter += size;
    }
  }

}


