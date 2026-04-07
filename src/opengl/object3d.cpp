#include "object3d.hpp"

#include <cstdio>

#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <darray.hpp>

void object3d::loadObject3d(const char* filename)
{
  FILE* file = nullptr;
  file = fopen(filename, "rb");
  if (!file) {
      perror("failed to load file");
      return;
  }
  
  char buffer[256]{'\0'};
  darray<vec3> vertices;
  darray<vec3> normals;
  darray<vec2> texCoords;
  darray<vec3> faces;

  while(fgets(buffer, sizeof(buffer), file) != nullptr) {

      vec3 input;
      char type;
      bool smoothing{0};
      
      if (buffer[0] == 'v') {
	  sscanf(buffer, "%c %f %f %f", &type, &input[0], &input[1], &input[2]);
	  vertices.push(input);
      }
      else if (buffer[0] == 'v' && buffer[1] == 'n') {
	  sscanf(buffer, "%c %f %f %f", &type, &input[0], &input[1], &input[2]);
	  normals.push(input);
      }
      else if (buffer[0] == 'v' && buffer[1] == 't') {
	  sscanf(buffer, "%c %f %f", &type, &input[0], &input[1]);
	  texCoords.push(vec2{input[0], input[1]});
      }
      else if (buffer[0] == 'f') {
    
	  char * ptr = buffer + 1;
	  u32 counter{0};
	  vec4 faces{};
	  
	  while((*ptr) != '\0') {

	      if ((*ptr) == ' ') {
		  ptr++;
		  continue;
	      } else {
		  char* end;
		  float val = strtod(ptr, &end);
		  faces[counter++] = val;
	      
		  if (ptr == end) {
		      ptr++;
		      continue;
		  }
	      
		  ptr = end;
	      }
	  }

	  //if 4 faces edge case
	  if (counter == 3) {
	      //TODO: start here
	  }
      }
  }

  

  fclose(file);
}
