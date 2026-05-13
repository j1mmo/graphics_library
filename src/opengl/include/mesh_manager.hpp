#ifndef MESH_MANAGER_HPP
#define MESH_MANAGER_HPP

#include "types.hpp"
#include "mesh.hpp"
#include <array.hpp>

namespace Mesh {
  struct Mesh_Manager {
    static constexpr u32 MAX_SIZE{64};
    array<u32, MAX_SIZE> vertex_arrays;
    array<u32, MAX_SIZE> buffers;
    u32 vertex_array_index{0};
    u32 buffers_index{0};
    
    void Init();
    [[nodiscard]] Mesh::Handles Create_Handle();
    void Destroy();
  };

}

#endif // MESH_MANAGER_HPP
