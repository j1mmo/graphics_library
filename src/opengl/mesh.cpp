#include <mesh.hpp>

#include <cmath>

#include <array.hpp>
#include <general.hpp>
#include <frenet_serret.hpp>

void mesh::generate_vertex_array_attributes(const mesh::Handles& mesh, const darray<float> data, const Attributes& attributes) {
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

void mesh::bind_vao(const mesh::Handles& handle) {
  glBindVertexArray(handle.vao);
}

void mesh::bind_vbo(const mesh::Handles& handle) {
  glBindBuffer(GL_ARRAY_BUFFER, handle.vbo);
}

void mesh::bind_ebo(const mesh::Handles& handle)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.ebo);
}

void mesh::generate_ebo(const mesh::Handles& m, const darray<u32>& indicies) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indicies.size(), indicies._data, GL_STATIC_DRAW);
}

void mesh::generate_ebo_stream(const mesh::Handles& m, const darray<u32>& indicies) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indicies.size(), indicies._data, GL_STREAM_DRAW);
}

void mesh::generate_vbo(const mesh::Handles& handle, const darray<float>& data) {
  glBindBuffer(GL_ARRAY_BUFFER, handle.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * data.size(), data._data, GL_STATIC_DRAW);
}

void mesh::generate_vbo_stream(const mesh::Handles& handle, const darray<float>& data) {
  glBindBuffer(GL_ARRAY_BUFFER, handle.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * data.size(), data._data, GL_STREAM_DRAW);
}

void mesh::draw_element_array(const mesh::Handles& mesh) {
  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
  glDrawElements(GL_TRIANGLES, mesh.drawElementsCount, GL_UNSIGNED_INT, 0);
}

void mesh::set_vertex_attributes(const Attributes& attributes) {
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

void mesh::generate_square(mesh::Handles& mesh) {
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

  mesh::Attributes attributes = {
    .strideLength = 3,
    .data = { 3 },
  };

  bind_vao(mesh);
  generate_vbo(mesh, square);
  generate_ebo(mesh, indicies);
  set_vertex_attributes(attributes);
  mesh.drawElementsCount = indicies.size();
}

void mesh::Generate_Cone(mesh::Handles& mesh) {
  constexpr i32 slices{32};
  constexpr f32 radius{1.0f};
  constexpr f32 height{1.0f};

  darray<f32> vertices;
  // tip first
  vertices.push(0.0f);
  vertices.push(height);
  vertices.push(0.0f);

  for (i32 i{0}; i <= slices; i++) {
      const f32 angle = (i * 2.0f * maths::pi) / static_cast<f32>(slices);
      const f32 x = radius * cos(angle);
      const f32 z = radius * sin(angle);

      vertices.push(x);
      vertices.push(0.0f);
      vertices.push(z);
  }

  darray<u32> indices;

  for (i32 i{0}; i < slices; i++) {
      i32 current_rim_index{i + 2};
      i32 next_rim_index{i + 3};

      if (i == slices - 1) {
	  next_rim_index = 2;
      }
      
      indices.push(0);
      indices.push(current_rim_index);
      indices.push(next_rim_index);
  }

  for (i32 i{0}; i < slices; i++) {
      int current_rim_index{i + 2};
      int next_rim_index{i + 3};

      if (i == slices - 1) {
	  next_rim_index = 2;
      }

      indices.push(1);
      indices.push(next_rim_index);
      indices.push(current_rim_index);
  }
  
  bind_vao(mesh);
  generate_vbo(mesh, vertices);
  generate_ebo(mesh, indices);
  set_vertex_attributes(mesh::Attributes{
      .strideLength = 3,
      .data = { 3 },
  });
  mesh.drawElementsCount = indices.size();
  indices.clean();
  vertices.clean();
}

void mesh::generate_outer(mesh::Handles& mesh) {
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
  set_vertex_attributes(mesh::Attributes{
      .strideLength = 3,
      .data = { 3 },
  });
  mesh.drawElementsCount = indicies.size();  
}

void mesh::generate_bending_tube(mesh::Handles& mesh)
{
  darray<f32> vertices;
  darray<u32> indices;

  constexpr f32 bend_radius = 0.5f; 
  constexpr f32 pipe_radius = 0.5f;
  constexpr i32 bend_segments = 20;
  constexpr i32 tube_segments = 20;

  constexpr f32 pivot_x = -0.5f;
  constexpr f32 pivot_y =  0.0f;
  constexpr f32 pivot_z = -0.5f;
  
  // Direction multiplier: 1.0f for Right, -1.0f for Left
  constexpr f32 direction = 1.0f; 

  // generate vertices
  for (i32 i = 0; i <= bend_segments; ++i) {
      const f32 bend_progress = static_cast<f32>(i) / bend_segments;
    
      // The angle of the turn (multiplied by direction)
      const f32 bend_angle = bend_progress * (3.14159f / 2.0f) * direction;
      const f32 bend_cos = cos(bend_angle);
      const f32 bend_sin = sin(bend_angle);

      for (i32 j = 0; j <= tube_segments; ++j) {
	  const f32 tube_progress = static_cast<f32>(j) / tube_segments;
	  const f32 ring_angle = tube_progress * (2.0f * 3.14159f);
        
	  const f32 ring_cos = cos(ring_angle);
	  const f32 ring_sin = sin(ring_angle);

	  const f32 local_x = (bend_radius + pipe_radius * ring_cos);
	  const f32 local_y = (pipe_radius * ring_sin); 
	  const f32 local_z = 0.0f; 

	  const f32 rotated_x = local_x * bend_cos - local_z * bend_sin;
	  const f32 rotated_z = local_x * bend_sin + local_z * bend_cos;

	  vertices.push(rotated_x + pivot_x);
	  vertices.push(local_y + pivot_y);
	  vertices.push(rotated_z + pivot_z);
      }
  }
  
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
  set_vertex_attributes(mesh::Attributes{
      .strideLength = 3,
      .data = { 3 },
    });
  mesh.drawElementsCount = indices.size();
  indices.clean();
  vertices.clean();
}

void mesh::generate_tube(mesh::Handles& mesh, const snake::Player& player) {
  constexpr u32 segments{16};
  constexpr u32 no_of_verts{segments};
  constexpr f32 theta{360.0f / (f32)segments};
  constexpr f32 radius{0.5};
  constexpr u32 vertices_length{no_of_verts * 3};
  constexpr u32 polygon_vertices_length{segments * 3};
  
  darray<f32> vertices;
  vertices.reserve(vertices_length * 2);

  u32 i{0};
  for (f32 point_coord{0.0f};
      i < no_of_verts && point_coord < 360.0f;
      i++, point_coord += theta) {
      const f32 radians = maths::radians(point_coord);
      
      vertices[i * 6]     = (f32)(radius * cos(radians));
      vertices[i * 6 + 1] = (f32)(radius * sin(radians));
      vertices[i * 6 + 2] = 0.5;

      vertices[i * 6 + 3] = (f32)(radius * cos(radians));
      vertices[i * 6 + 4] = (f32)(radius * sin(radians));
      vertices[i * 6 + 5] = -0.5;
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
  set_vertex_attributes(mesh::Attributes{
      .strideLength = 3,
      .data = { 3 },
    });
  mesh.drawElementsCount = polygons.size();
  polygons.clean();
  vertices.clean();
}

void mesh::generate_circle(mesh::Handles& mesh)
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
  set_vertex_attributes(mesh::Attributes{
      .strideLength = 3,
      .data = { 3 },
    });
  mesh.drawElementsCount = polygon_vertices_length;
  polygons.clean();
  vertices.clean();
}
