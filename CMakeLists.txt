cmake_minimum_required(VERSION 3.0)

project(elliptic_curve)

find_package(OpenGL REQUIRED)

link_directories(libs/GLFW)
include_directories(libs/GLFW)
include_directories(libs/imgui)
include_directories(include)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/*.cpp)
add_executable(elliptic_curve ${LIBS} ${SOURCES})

target_link_libraries(elliptic_curve PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(elliptic_curve PRIVATE glfw)
