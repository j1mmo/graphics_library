set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_executable(opengl
	glad.c
	main.cpp
	vertex_array.cpp	
	shader.cpp
	texture.cpp)

target_include_directories(opengl PUBLIC
				 ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(opengl PRIVATE
				  ${CMAKE_SOURCE_DIR}/external/spdlog/include
				  ${CMAKE_SOURCE_DIR}/external/maths/src/maths/include)

target_link_libraries(opengl PUBLIC spdlog::spdlog_header_only PRIVATE maths glfw dl)
