set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp	
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

add_executable(opengl
	glad.c
	main.cpp
	init.cpp
	vertex_array.cpp	
	shader.cpp
	texture.cpp
	camera.cpp
	mesh.cpp
	mesh_manager.cpp
	object3d.cpp
	obj_loader.cpp
	snake.cpp)

add_library(imgui ${IMGUI_SOURCES})

target_include_directories(opengl PUBLIC
				 ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(opengl PRIVATE
				  ${CMAKE_SOURCE_DIR}/external/spdlog/include
				  ${CMAKE_SOURCE_DIR}/external/maths/src/maths/include
				  ${CMAKE_SOURCE_DIR}/external/dsa/src/dsa/include
				  ${IMGUI_DIR}
    				  ${IMGUI_DIR}/backends)

target_include_directories(imgui PUBLIC ${IMGUI_DIR} PRIVATE ${IMGUI_DIR}/backends)

target_link_libraries(opengl PUBLIC spdlog::spdlog_header_only PRIVATE dsa maths glfw dl imgui)
