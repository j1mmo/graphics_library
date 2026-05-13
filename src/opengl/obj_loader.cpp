#include <obj_loader.hpp>

#include <spdlog/spdlog.h>

#include <darray.hpp>
#include <array.hpp>

#include <types.hpp>
#include <mesh.hpp>

#include <cstdlib>

void Load_Obj(const char* file_location, Mesh::Handles& handle)
{
  FILE* file{nullptr};
  file = fopen(file_location, "r");

  if (nullptr == file) {
      spdlog::error("Unable to open obj file: {}", file_location);
      return;
  }

  char* line{nullptr};
  size_t length{0};
  bool smoothing{false};
  
  darray<f32> vertices;
  darray<f32> normals;
  darray<f32> uv;
  
  darray<u32> vertices_index;
  darray<u32> uvs_index;
  darray<u32> normals_index;
  
  while(1) {
      ssize_t read = getline(&line, &length, file);

      if (-1 == read || length <= 0) {
	  break;
      }

      char line_tag{line[0]};

      switch(line_tag) {
      case 'v':
      case 'V': {
	  if (length > 2) {
	      char second_character{line[1]};
	      if (second_character == 't') {
		  array<f32, 2> temp_uv;
		  char u{}, v{};
		  int items = sscanf(line, "%c%c %f %f",
				     &u,
				     &v,
				     &temp_uv[0],
				     &temp_uv[1]);
		  if (4 != items) {
		      spdlog::error("Error parsing line: {}", line);
		  }
		  uv.push(temp_uv[0]);
		  uv.push(temp_uv[1]);
	      }
	      else if (second_character == ' ') {
		  array<f32, 3> temp_vertex;
		  char v{};
		  int items = sscanf(line, "%c %f %f %f",
				     &v,
				     &temp_vertex[0],
				     &temp_vertex[1],
				     &temp_vertex[2]);
		  if (4 != items) {
		      spdlog::error("Error parsing line: {}", line);
		  }
		  vertices.push(temp_vertex[0]);
		  vertices.push(temp_vertex[1]);
		  vertices.push(temp_vertex[2]);
	      }
	      else if (second_character == 'n') {
		  array<f32, 3> temp_normal;
		  char v{}, n{};
		  int items = sscanf(line, "%c%c %f %f %f",
				     &v,
				     &n,
				     &temp_normal[0],
				     &temp_normal[1],
				     &temp_normal[2]);
		  if (5 != items) {
		      spdlog::error("Error parsing line: {}", line);
		  }
		  normals.push(temp_normal[0]);
		  normals.push(temp_normal[1]);
		  normals.push(temp_normal[2]);
	      }
	  }
      } break;
      case 's':
      case 'S': {
	  if (length > 3 && line[1] == ' ') {
	      int smoothing_value = atoi(line + 2);
	      smoothing = static_cast<bool>(smoothing_value);
	  }
      } break;

      case 'f':
      case 'F': {
	  array<u32, 3> vertex_index;
	  array<u32, 3> uv_index;
	  array<u32, 3> normal_index;
	  char f{};
	  int matches = sscanf(line,
			       "%c %u/%u/%u %u/%u/%u %u/%u/%u\n", &f,
			       &vertex_index[0], &uv_index[0], &normal_index[0],
			       &vertex_index[1], &uv_index[1], &normal_index[1],
			       &vertex_index[2], &uv_index[2], &normal_index[2]);

	  if (matches != 10){
	      printf("faces need to be triangulated in the form: \n"
		     "v/v/v vn/vn/vn uv/uv/uv\n");
	  }
	  
	  vertices_index.push(vertex_index[0] - 1);
	  vertices_index.push(vertex_index[1] - 1);
	  vertices_index.push(vertex_index[2] - 1);
	  uvs_index     .push(uv_index[0] - 1);
	  uvs_index     .push(uv_index[1] - 1);
	  uvs_index     .push(uv_index[2] - 1);
	  normals_index .push(normal_index[0] - 1);
	  normals_index .push(normal_index[1] - 1);
	  normals_index .push(normal_index[2] - 1);
	  
      } break;
      }
      
  }
  free(line);

  u32 vertices_count = vertices_index.size();

  darray<f32> out_data; // ebo
  for(u32 index{0}; index < vertices_count; index++ ) {
      u32 vertices_i = vertices_index[index];
      u32 normals_i = normals_index[index];
      u32 uvs_i = uvs_index[index];

      out_data.push(vertices[vertices_i * 3]);
      out_data.push(vertices[vertices_i * 3 + 1]);
      out_data.push(vertices[vertices_i * 3 + 2]);

      out_data.push(normals[normals_i * 3]);
      out_data.push(normals[normals_i * 3 + 1]);
      out_data.push(normals[normals_i * 3 + 2]);

      out_data.push(uv[uvs_i * 2]);
      out_data.push(uv[uvs_i * 2 + 1]);

  }

  vertices.clean();
  normals.clean();
  uv.clean();

  vertices_index.clean();
  uvs_index.clean();
  normals_index.clean();

  bind_vao(handle);
  generate_vertex_array_attributes(handle, out_data,
				   Mesh::Attributes{
				     .strideLength = 8,
				     .data = { 3, 3, 2 },});
				   
  handle.drawElementsCount = out_data.size();
  out_data.clean();
}
