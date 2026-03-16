cmake_minimum_required(VERSION 3.16)
project(opengl)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(external/spdlog)
add_subdirectory(external/maths)

add_executable(opengl src/glad.c
		      src/main.cpp
		      include/types.hpp
		      include/stb_image.h
		      
		      include/shader.hpp
		      include/texture.hpp
		      
		      src/shader.cpp
		      src/texture.cpp
		      )

target_include_directories(opengl PRIVATE include shaders external/maths/include)
		      
target_link_libraries(opengl PUBLIC spdlog::spdlog_header_only PRIVATE maths glfw dl)

