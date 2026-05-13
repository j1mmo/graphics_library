#include <mesh_manager.hpp>

#include <glad/glad.h>

void Mesh::Mesh_Manager::Init()
{
  glGenVertexArrays(Mesh::Mesh_Manager::MAX_SIZE, vertex_arrays.data());
  glGenBuffers(Mesh::Mesh_Manager::MAX_SIZE, buffers.data());
}

Mesh::Handles Mesh::Mesh_Manager::Create_Handle()
{
  Mesh::Handles temp {
    .vao = vertex_arrays[vertex_array_index],
    .vbo = buffers[buffers_index],
    .ebo = buffers[buffers_index + 1],
    .drawElementsCount = 0
  };
  vertex_array_index++;
  buffers_index += 2;
  return temp;
}

void Mesh::Mesh_Manager::Destroy()
{
  glDeleteVertexArrays(Mesh::Mesh_Manager::MAX_SIZE, vertex_arrays.data());
  glDeleteBuffers(Mesh::Mesh_Manager::MAX_SIZE, buffers.data());
}
