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
			    attributes.strideLength * sizeof(float),
			    (void*) (offsetCounter * sizeof(float))
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
  darray<float> vertices;
  darray<u32> indices;

  float bend_radius = 0.5f; // Bend radius
  float pipe_radius = 0.5f; // Pipe radius
  int bend_segments = 10;
  int tube_segments = 6;

  // pivot points
  const float pivotX = -0.5f;
  const float pivotY =  0.0f;
  const float pivotZ = -0.5f;
  // Direction multiplier: 1.0f for Right, -1.0f for Left
  float direction = 1.0f; 

  for (int i = 0; i <= bend_segments; ++i) {
      float bendProgress = (float)i / bend_segments;
    
      // The angle of the turn (multiplied by direction)
      float bendAngle = bendProgress * (3.14159f / 2.0f) * direction;
      float bendCos = cos(bendAngle);
      float bendSin = sin(bendAngle);

      for (int j = 0; j <= tube_segments; ++j) {
	  float tubeProgress = (float)j / tube_segments;
	  float ringAngle = tubeProgress * (2.0f * 3.14159f);
        
	  float ringCos = cos(ringAngle);
	  float ringSin = sin(ringAngle);

	  // 1. Define the point relative to the pivot
	  // We assume the pipe starts at the pivot + bendRadius
	  float localX = (bend_radius + pipe_radius * ringCos);
	  float localY = (pipe_radius * ringSin); 
	  float localZ = 0.0f; 

	  // 2. Rotate the point around the Pivot's Y-axis
	  // Standard 2D rotation:
	  // x' = x*cos - z*sin
	  // z' = x*sin + z*cos
	  float rotatedX = localX * bendCos - localZ * bendSin;
	  float rotatedZ = localX * bendSin + localZ * bendCos;

	  // 3. Translate the rotated point to the Pivot's world position
	  vertices.push(rotatedX + pivotX);
	  vertices.push(localY + pivotY);
	  vertices.push(rotatedZ + pivotZ);
      }
  }

  for (int i = 0; i < bend_segments; ++i) {
      for (int j = 0; j < tube_segments; ++j) {
	  int first = (i * (tube_segments + 1)) + j;
	  int second = first + tube_segments + 1;

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
  const u32 segments{8};
  const u32 no_of_verts{segments};
  const f32 theta{360.0f / (f32)segments};
  const f32 radius{0.5};
  const u32 vertices_length{no_of_verts * 3};
  const u32 polygon_vertices_length{segments * 3};
  
  darray<float> vertices;
  vertices.reserve(vertices_length * 2);

  u32 i{0};
  for (float point_coord{0.0f};
      i < no_of_verts && point_coord < 360.0f;
      i++, point_coord += theta) {
      const f32 radians = maths::radians(point_coord);
      vertices[i * 6]     = (float)(radius * cos(radians));
      vertices[i * 6 + 1] = (float)(radius * sin(radians));
      vertices[i * 6 + 2] = 0.5;

      vertices[i * 6 + 3] = (float)(radius * cos(radians));
      vertices[i * 6 + 4] = (float)(radius * sin(radians));
      vertices[i * 6 + 5] = -0.5;
  }

  darray<u32> polygons;
  for (int i = 0; i < segments; i++) {
    
    int currentFront = i * 2;
    int currentBack  = i * 2 + 1;
    
    // Next segment (using mod to wrap from the last vertex back to the first)
    int nextFront = ((i + 1) % segments) * 2;
    int nextBack  = ((i + 1) % segments) * 2 + 1;

    // Triangle 1
    polygons.push(currentFront);
    polygons.push(nextBack);
    polygons.push(currentBack);

    // Triangle 2
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
  const u32 segments{20};
  const u32 no_of_verts{segments + 1};
  const f32 theta{360.0f / (f32)segments};
  const f32 radius{0.5};
  const u32 vertices_length{no_of_verts * 3};
  const u32 polygon_vertices_length{segments * 3};
  
  darray<float> vertices;
  vertices.reserve(vertices_length);
  //set the center of the circle
  vertices[0] = 0.0f;
  vertices[1] = 0.0f;
  vertices[2] = 0.0f;
  // therefore start from 1
  u32 i{1};
  // create the points of the circle
  for(float point_coord{0.0f};
      i < no_of_verts && point_coord < 360.0f;
      i++, point_coord += theta) {
      const f32 radians = maths::radians(point_coord);
      vertices[i * 3]     = (float)(radius * sin(radians));
      vertices[i * 3 + 1] = (float)(radius * cos(radians));
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
