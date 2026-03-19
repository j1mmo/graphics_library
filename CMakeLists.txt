cmake_minimum_required(VERSION 3.16)
project(opengl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CSS_STANDARD_REQUIRED ON)
set(CMAKE_CSS_STANDARD OFF)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(BUILD_TESTS OFF CACHE BOOL "Disable building tests" FORCE)

add_subdirectory(external/spdlog)
add_subdirectory(external/mathslib)
add_subdirectory(src)

