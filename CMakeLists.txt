cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(OpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)






# glfw
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/third_party/glfw)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
include_directories(${GLFW_DIR}/include)
add_subdirectory(${GLFW_DIR})


# glad
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/third_party/glad)
include_directories(${GLAD_DIR})
add_library(glad ${GLAD_DIR}/glad.c)


# stb
set(STB_DIR ${CMAKE_SOURCE_DIR}/third_party/stb)
include_directories(${STB_DIR})
add_library(stb ${CMAKE_SOURCE_DIR}/third_party/stb_image.cpp)

# imgui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)
include_directories(${IMGUI_DIR})
add_library("imgui" "${IMGUI_DIR}/imgui.cpp"
                  "${IMGUI_DIR}/imgui_demo.cpp"
                  "${IMGUI_DIR}/imgui_draw.cpp"
                  "${IMGUI_DIR}/imgui_tables.cpp"
                  "${IMGUI_DIR}/imgui_widgets.cpp"
                  "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
                  "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
                  "${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp"
                  )



if(WIN32)
      set(LIBS glfw glad stb imgui opengl32)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
      INCLUDE_DIRECTORIES(/System/Library/Frameworks)
      FIND_LIBRARY(COCOA_LIBRARY Cocoa)
      FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
      FIND_LIBRARY(IOKit_LIBRARY IOKit)
      FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
      MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
      SET(LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} gflags glfw glad stb imgui ${CoreVideo_LIBRARY})
endif(WIN32)



add_subdirectory(${CMAKE_SOURCE_DIR}/src/0.hello_world)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/1.1.hello_window)