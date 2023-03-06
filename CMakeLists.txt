cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(OpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)



add_subdirectory(${CMAKE_SOURCE_DIR}/src/0.hello_world)


# glfw

set(GLFW_DIR ${CMAKE_SOURCE_DIR}/third_party/glfw-3.3.8)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
include_directories(${GLFW_DIR}/include)
add_subdirectory(${GLFW_DIR})


# glad

# stb

# imgui
