cmake_minimum_required(VERSION 3.16)
project(opengl)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(BUILD_TESTS OFF CACHE BOOL "Disable building tests" FORCE)

add_subdirectory(external/spdlog)

add_executable(opengl src/glad.c
		      src/main.cpp
		      include/types.hpp
		      include/stb_image.h
		      
		      include/shader.hpp
		      include/texture.hpp
		      
		      src/shader.cpp
		      src/texture.cpp
		      )

target_include_directories(opengl PRIVATE include shaders ${CMAKE_SOURCE_DIR}/external/maths/include)
		      
target_link_libraries(opengl PUBLIC spdlog::spdlog_header_only PRIVATE maths glfw dl)

