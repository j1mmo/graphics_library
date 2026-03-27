#include <mesh.hpp>


/**
 * add option for ebo? 
template<u32 vertexDataLength, u32 indiciesDataLength, u32 attribSize>
void mesh::generate(const mesh& mesh, const array<float, vertexDataLength>& vertexData, const array<u32, indiciesDataLength> indicesData, const array<float, attribSize>& attribute) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indiciesData.size(), indiciesData._data, GL_STATIC_DRAW);

  
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
  glBufferData(GL_ARRAY_BUFFER, dataSize, dataPointer, GL_STATIC_DRAW);

  i32 offsetCounter{0};
  for (u32 attribIndex{0}; attribIndex < attribSizeCount; attribIndex++) {
      u32 size = attributeSize[attribSizeCount];
      glVertexAttribPointer(attribIndex, *attributeSize, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*) (offsetCounter * sizeof(float)));
      glEnableVertexAttribArray(attribIndex);
      offsetCounter += size;
  }
}

*/
