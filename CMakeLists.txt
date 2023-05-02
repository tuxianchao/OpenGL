cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(OpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

Message(${CMAKE_BUILD_TYPE})

# output
if(WIN32)
    add_definitions(/W4 /D_CRT_SECURE_NO_WARNINGS)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

    option(USE_SOLUTION_FOLDERS "使用资源管理器文件夹" ON)
    if(USE_SOLUTION_FOLDERS)
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
        set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
    endif()
endif(WIN32)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_DEBUG_POSTFIX "_debug")
set(CMAKE_RELEASE_POSTFIX "_release")


configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# spdlog
set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/third_party/spdlog)
include_directories(${SPDLOG_DIR}/include)
add_subdirectory(${SPDLOG_DIR})

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

#glm
set(GLM_DIR ${CMAKE_SOURCE_DIR}/third_party/glm)
include_directories(${GLM_DIR})
add_subdirectory(${GLM_DIR})

# stb
set(STB_DIR ${CMAKE_SOURCE_DIR}/third_party/stb)
include_directories(${STB_DIR})
add_library(stb ${CMAKE_SOURCE_DIR}/third_party/stb_image.cpp)

# imgui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)
add_library("imgui" "${IMGUI_DIR}/imgui.cpp"
                  "${IMGUI_DIR}/imgui_demo.cpp"
                  "${IMGUI_DIR}/imgui_draw.cpp"
                  "${IMGUI_DIR}/imgui_tables.cpp"
                  "${IMGUI_DIR}/imgui_widgets.cpp"
                  "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
                  "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
                  "${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp"
                  )

# assimp
set(ASSIMP_DIR ${CMAKE_SOURCE_DIR}/third_party/assimp)
include_directories(${ASSIMP_DIR}/include)
add_subdirectory(${ASSIMP_DIR})

if(WIN32)
      set(LIBS spdlog glfw glad glm stb imgui assimp opengl32)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
      INCLUDE_DIRECTORIES(/System/Library/Frameworks)
      FIND_LIBRARY(COCOA_LIBRARY Cocoa)
      FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
      FIND_LIBRARY(IOKit_LIBRARY IOKit)
      FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
      MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
      SET(LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} spdlog glfw glad glm stb imgui assimp ${CoreVideo_LIBRARY})
endif(WIN32)



macro(makeLink src dest target)
  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()



# config_target
function(config_target target_name target_dir)
    set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin/${target_name})
    if(WIN32)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
        set_property(TARGET ${target_name} PROPERTY  FOLDER ${target_dir})
        set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
        set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${BIN_DIR}/${CMAKE_BUILD_TYPE}")
    elseif(APPLE)
        set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
        set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}/immediate/Debug")
        set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}/immediate/Release")
        set_target_properties(${target_name} PROPERTIES XCODE_GENERATE_SCHEME TRUE
                                    XCODE_SCHEME_WORKING_DIRECTORY ${BIN_DIR})
        if(EXISTS ${BIN_DIR})
        else()
            make_directory(${BIN_DIR})
        endif()
    endif(WIN32)

    # 拷贝shader
    file(GLOB SHADERS
    "*.vs"
    "*.frag"
    "*.fs"
    "*.gs"
    "*.cs"
    )

    foreach(SHADER ${SHADERS})
        if(WIN32)
            # configure_file(${SHADER} "test")
            message(copy ${SHADER})
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
        elseif(APPLE)
            # create symbolic link for *.vs *.fs *.gs
            get_filename_component(SHADERNAME ${SHADER} NAME)
            makeLink(${SHADER} ${BIN_DIR}/${SHADERNAME} ${NAME})
            message(STATUS "makeLink ${SHADER} => ${BIN_DIR}/${SHADERNAME}")
        endif(WIN32)
    endforeach(SHADER)
endfunction()


# lib test
add_subdirectory(${CMAKE_SOURCE_DIR}/src/test_spdlog)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/test_imgui)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/test_root_dir)

# course chapter 1
add_subdirectory(${CMAKE_SOURCE_DIR}/src/0.hello_world)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/1.1.hello_window)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/1.2.hello_window_clear)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/2.1.hello_triangle)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/2.2.hello_triangle_indexed)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/2.3.hello_triangle_exercise1)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/2.4.hello_triangle_exercise2)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/2.5.hello_triangle_exercise3)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/3.1.shaders_uniform)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/3.2.shaders_interpolation)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/3.3.shaders_class)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/3.4.shaders_exercise1)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/3.5.shaders_exercise2)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/3.6.shaders_exercise3)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/4.1.textures)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/4.2.textures_combined)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/4.3.textures_exercise1)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/4.4.textures_exercise2)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/4.5.textures_exercise3)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/4.6.textures_exercise4)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/5.1.transformations)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/5.2.transformations_exercise1)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/5.3.transformations_exercise2)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/6.1.coordinate_systems)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/6.1.coordinate_systems_control)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/6.2.coordinate_systems_depth)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/6.3.coordinate_systems_multiple)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/6.4.coordinate_systems_exercise)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/7.1.camera_circle)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/7.2.camera_keyboard_dt)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/7.2.camera_keyboard_dt_cursor)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/7.3.camera_keyboard_zoom)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/7.4.camera_class)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/7.5.camera_exercise1)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/1.getting_started/7.6.camera_exercise2)



# course chapter 2
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/1.colors)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/2.1.basic_lighting_diffuse)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/2.2.basic_lighting_specular)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/2.3.basic_lighting_practice1)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/2.4.basic_lighting_practice2)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/2.5.basic_lighting_practice3)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/2.6.basic_lighting_practice4)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/3.1.materials)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/3.2.materials_practice)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/4.1.light_map_diffuse)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/4.2.light_map_specular)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/4.3.light_map_practice2)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/4.4.light_map_practice3)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/4.5.light_map_practice4)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/2.lighting/5.1.light_casters_directional)
# course chapter 3
