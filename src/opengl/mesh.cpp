#include <mesh.hpp>

#include <cmath>

#include <array.hpp>
#include <general.hpp>
#include <frenet_serret.hpp>

void Mesh::generate_vertex_array_attributes(const Mesh::Handles& mesh, const darray<float> data, const Attributes& attributes) {
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

void Mesh::bind_vao(const Mesh::Handles& handle) {
  glBindVertexArray(handle.vao);
}

void Mesh::bind_vbo(const Mesh::Handles& handle) {
  glBindBuffer(GL_ARRAY_BUFFER, handle.vbo);
}

void Mesh::bind_ebo(const Mesh::Handles& handle)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.ebo);
}

void Mesh::generate_ebo(const Mesh::Handles& m, const darray<u32>& indicies) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indicies.size(), indicies._data, GL_STATIC_DRAW);
}

void Mesh::generate_ebo_stream(const Mesh::Handles& m, const darray<u32>& indicies) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indicies.size(), indicies._data, GL_STREAM_DRAW);
}

void Mesh::generate_vbo(const Mesh::Handles& handle, const darray<float>& data) {
  glBindBuffer(GL_ARRAY_BUFFER, handle.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * data.size(), data._data, GL_STATIC_DRAW);
}

void Mesh::generate_vbo_stream(const Mesh::Handles& handle, const darray<float>& data) {
  glBindBuffer(GL_ARRAY_BUFFER, handle.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * data.size(), data._data, GL_STREAM_DRAW);
}

void Mesh::draw_element_array(const Mesh::Handles& mesh) {
  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
  glDrawElements(GL_TRIANGLES, mesh.drawElementsCount, GL_UNSIGNED_INT, 0);
}

void Mesh::draw_vertex_arrays(const Mesh::Handles& mesh) {
  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
  glDrawArrays(GL_TRIANGLES, 0, mesh.drawElementsCount);
}

void Mesh::set_vertex_attributes(const Attributes& attributes) {
  i32 offsetCounter{0}, index{0};
    
  for (const u32 attributeSize : attributes.data) {
      glVertexAttribPointer(
			    index,
			    attributeSize,
			    GL_FLOAT,
			    GL_FALSE,
			    attributes.strideLength * sizeof(f32),
			    (void*) (offsetCounter * sizeof(f32))
      );
      glEnableVertexAttribArray(index);
      offsetCounter += attributeSize;
      index++;
  }
}

void Mesh::generate_square(Mesh::Handles& mesh) {
  const static darray<float> square = {
    0.4f,  0.0f, 0.4f,  // top right - 0
    0.4f,  0.0f,-0.4f,  // bottom right - 1
    -0.4f,  0.0f,-0.4f,  // bottom left - 2
    -0.4f,  0.0f, 0.4f   // top left - 3
  };
  
  const static darray<u32> indicies{
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };  

  Mesh::Attributes attributes = {
    .strideLength = 3,
    .data = { 3 },
  };

  bind_vao(mesh);
  generate_vbo(mesh, square);
  generate_ebo(mesh, indicies);
  set_vertex_attributes(attributes);
  mesh.drawElementsCount = indicies.size();
}

void Mesh::Generate_Cone(Mesh::Handles& mesh) {
  constexpr u32 segments{16};
  constexpr u32 no_of_verts{segments};
  constexpr f32 theta{(360.0f / static_cast<f32>(segments))};
  constexpr f32 radius{0.5};
  constexpr u32 vertices_length{no_of_verts * 3};
  constexpr u32 polygon_vertices_length{segments * 3};
  
  darray<f32> vertices;
  
  for (u32 i{0}; i < no_of_verts; i++) {
      const f32 point_coord = static_cast<f32>(i) * theta;
      const f32 radians = maths::radians(point_coord);
      const f32 cos_theta = (cos(radians));
      const f32 sin_theta = (sin(radians));
      const f32 u_value = static_cast<f32>(i) / static_cast<f32>(segments);
      
      //vertices x, y, z
      vertices.push(radius * cos(radians));
      vertices.push(radius * sin(radians));
      vertices.push(0.5f);
      
      //normals
      vertices.push(cos_theta);
      vertices.push(sin_theta);
      vertices.push(0.0f);

      //uv
      vertices.push(u_value);
      vertices.push(0.0f);
      
      //vertices x, y, z
      vertices.push(0.2 * cos(radians));
      vertices.push(0.2 * sin(radians));
      vertices.push(-0.5);

      //normals
      vertices.push(cos_theta);
      vertices.push(sin_theta);
      vertices.push(0.0f);

      //uv
      vertices.push(u_value);
      vertices.push(0.0f);
  }

  darray<u32> polygons;
  for (i32 i = 0; i < segments; i++) {
    
    const i32 currentFront = i * 2;
    const i32 currentBack  = i * 2 + 1;
    
    // mod to wrap from the last vertex back to the first
    const i32 nextFront = ((i + 1) % segments) * 2;
    const i32 nextBack  = ((i + 1) % segments) * 2 + 1;

    // triangle 1
    polygons.push(currentFront);
    polygons.push(nextBack);
    polygons.push(currentBack);

    // triangle 2
    polygons.push(currentFront);
    polygons.push(nextFront);
    polygons.push(nextBack);
  }

  bind_vao(mesh);
  generate_vbo(mesh, vertices);
  generate_ebo(mesh, polygons);
  set_vertex_attributes(Mesh::Attributes{
      .strideLength = 8,
      .data = { 3, 3, 2 },
    });
  mesh.drawElementsCount = polygons.size();
  polygons.clean();
  vertices.clean();
}

void Mesh::generate_outer(Mesh::Handles& mesh) {
  const static darray<float> square = {
     0.5f,  0.0f, 0.5f,  // top right - 0
     0.5f,  0.0f,-0.5f,  // bottom right - 1
    -0.5f,  0.0f,-0.5f,  // bottom left - 2
    -0.5f,  0.0f, 0.5f,   // top left - 3
     0.4f,  0.0f, 0.4f,  // top right inner - 4
     0.4f,  0.0f,-0.4f,  // bottom right inner - 5
    -0.4f,  0.0f,-0.4f,  // bottom left inner - 6
    -0.4f,  0.0f, 0.4f   // top left inner - 7
  };
  
  const static darray<u32> indicies{
      4, 0, 1, 
      4, 1, 5,
      5, 1, 2,
      5, 2, 6,
      3, 7, 6,
      3, 6, 2,
      3, 0, 4,
      3, 4, 7
  };

  bind_vao(mesh);
  generate_vbo(mesh, square);
  generate_ebo(mesh, indicies);
  set_vertex_attributes(Mesh::Attributes{
      .strideLength = 3,
      .data = { 3 },
  });
  mesh.drawElementsCount = indicies.size();  
}

void Mesh::generate_bending_tube(Mesh::Handles& mesh)
{
  constexpr f32 bend_radius = 0.5f; 
  constexpr f32 pipe_radius = 0.5f;
  constexpr i32 bend_segments = 6;
  constexpr i32 tube_segments = 6;
  constexpr f32 pivot_x = -0.5f;
  constexpr f32 pivot_y =  0.0f;
  constexpr f32 pivot_z = -0.5f;
  // Direction multiplier: 1.0f for Right, -1.0f for Left
  constexpr f32 direction = 1.0f; 

  darray<f32> vertices;
  // generate vertices
  for (i32 i = 0; i <= bend_segments; ++i) {
      const f32 bend_progress = static_cast<f32>(i) / bend_segments;
      // The angle of the turn (multiplied by direction)
      const f32 bend_angle = bend_progress * (maths::pi / 2.0f) * direction;
      const f32 bend_cos = cos(bend_angle);
      const f32 bend_sin = sin(bend_angle);

      for (i32 j = 0; j <= tube_segments; j++) {
	  const f32 tube_progress = static_cast<f32>(j) / tube_segments;
	  const f32 ring_angle = tube_progress * (2.0f * maths::pi);
	  
	  const f32 ring_cos = cos(ring_angle);
	  const f32 ring_sin = sin(ring_angle);

	  const f32 local_nx = ring_cos;
	  const f32 local_ny = ring_sin;
	  const f32 local_nz = 0.0f;

	  const f32 rotated_nx = local_nx * bend_cos - local_nz * bend_sin;
          const f32 rotated_ny = local_ny; // Y doesn't change in this rotation
          const f32 rotated_nz = local_nx * bend_sin + local_nz * bend_cos;

	  const f32 local_x = (bend_radius + pipe_radius * ring_cos);
	  const f32 local_y = (pipe_radius * ring_sin); 
	  const f32 local_z = 0.0f; 

	  const f32 rotated_x = local_x * bend_cos - local_z * bend_sin;
	  const f32 rotated_z = local_x * bend_sin + local_z * bend_cos;
	  //verts x, y, z
	  vertices.push(rotated_x + pivot_x);
	  vertices.push(local_y + pivot_y);
	  vertices.push(rotated_z + pivot_z);
	  //normals x, y, z
	  vertices.push(rotated_nx);
          vertices.push(rotated_ny);
          vertices.push(rotated_nz);
          // UVs
          vertices.push(tube_progress); // U
          vertices.push(bend_progress); // V
      }
  }

  darray<u32> indices;
  // generate indices
  for (i32 i = 0; i < bend_segments; ++i) {
      for (i32 j = 0; j < tube_segments; ++j) {
	  const i32 first = (i * (tube_segments + 1)) + j;
	  const i32 second = first + tube_segments + 1;

	  // Triangle 1
	  indices.push(first);
	  indices.push(second);
	  indices.push(first + 1);

	  // Triangle 2
	  indices.push(second);
	  indices.push(second + 1);
	  indices.push(first + 1);
      }
  }

  bind_vao(mesh);
  generate_vbo(mesh, vertices);
  generate_ebo(mesh, indices);
  set_vertex_attributes(Mesh::Attributes{
      .strideLength = 8,
      .data = { 3, 3, 2 },
    });
  mesh.drawElementsCount = indices.size();
  indices.clean();
  vertices.clean();
}

void Mesh::generate_tube(Mesh::Handles& mesh, const snake::Player& player) {
  constexpr u32 segments{16};
  constexpr u32 no_of_verts{segments};
  constexpr f32 theta{360.0f / static_cast<f32>(segments)};
  constexpr f32 radius{0.5};
  constexpr u32 vertices_length{no_of_verts * 3};
  constexpr u32 polygon_vertices_length{segments * 3};
  
  darray<f32> vertices;
  for (u32 i{0}; i < no_of_verts; i++) {
      const f32 point_coord = static_cast<f32>(i) * theta;
      const f32 radians = maths::radians(point_coord);
      const f32 cos_theta = (cos(radians));
      const f32 sin_theta = (sin(radians));
      const f32 u_value = static_cast<f32>(i) / static_cast<f32>(segments);
      
      vertices.push(radius * cos_theta);
      vertices.push(radius * sin_theta);
      vertices.push(0.5);

      //normals
      vertices.push(cos_theta);
      vertices.push(sin_theta);
      vertices.push(0.0f);

      //uv
      vertices.push(u_value);
      vertices.push(0.0f);

      vertices.push(radius * cos(radians));
      vertices.push(radius * sin(radians));
      vertices.push(-0.5);

      //normals
      vertices.push(cos_theta);
      vertices.push(sin_theta);
      vertices.push(0.0f);

      //uv
      vertices.push(u_value);
      vertices.push(0.0f);
  }

  darray<u32> polygons;
  for (i32 i = 0; i < segments; i++) {
    
    const i32 currentFront = i * 2;
    const i32 currentBack  = i * 2 + 1;
    
    // mod to wrap from the last vertex back to the first
    const i32 nextFront = ((i + 1) % segments) * 2;
    const i32 nextBack  = ((i + 1) % segments) * 2 + 1;

    // triangle 1
    polygons.push(currentFront);
    polygons.push(nextBack);
    polygons.push(currentBack);

    // triangle 2
    polygons.push(currentFront);
    polygons.push(nextFront);
    polygons.push(nextBack);
  }

  bind_vao(mesh);
  generate_vbo(mesh, vertices);
  generate_ebo(mesh, polygons);
  set_vertex_attributes(Mesh::Attributes{
      .strideLength = 8,
      .data = { 3, 3, 2 },
  });
  mesh.drawElementsCount = polygons.size();
  polygons.clean();
  vertices.clean();
}

void Mesh::generate_circle(Mesh::Handles& mesh)
{
  constexpr u32 segments{20};
  constexpr u32 no_of_verts{segments + 1};
  constexpr f32 theta{360.0f / (f32)segments};
  constexpr f32 radius{0.5};
  constexpr u32 vertices_length{no_of_verts * 3};
  constexpr u32 polygon_vertices_length{segments * 3};
  
  darray<f32> vertices;
  vertices.reserve(vertices_length);
  // set the center of the circle
  vertices[0] = 0.0f;
  vertices[1] = 0.0f;
  vertices[2] = 0.0f;
  
  // therefore start from 1
  u32 i{1};
  for(f32 point_coord{0.0f};
      i < no_of_verts && point_coord < 360.0f;
      i++, point_coord += theta) {
      const f32 radians = maths::radians(point_coord);
      vertices[i * 3]     = (f32)(radius * sin(radians));
      vertices[i * 3 + 1] = (f32)(radius * cos(radians));
      vertices[i * 3 + 2] = 0;
  }
      
  darray<u32> polygons;
  polygons.reserve(polygon_vertices_length);

  i = 0;
  while (i < segments) {
      polygons[i * 3] = 0;
      polygons[i * 3 + 1] = i + 2;
      polygons[i * 3 + 2] = i + 1;
      i++;
  }
  polygons[(i - 1) * 3 + 1] = 1;
  
  bind_vao(mesh);
  generate_vbo(mesh, vertices);
  generate_ebo(mesh, polygons);
  set_vertex_attributes(Mesh::Attributes{
      .strideLength = 3,
      .data = { 3 },
    });
  mesh.drawElementsCount = polygon_vertices_length;
  polygons.clean();
  vertices.clean();
}
