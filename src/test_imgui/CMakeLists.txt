set(NAME imgui_test)
file(GLOB SOURCE
    "./*.h"
    "./*.cpp"
)

add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} spdlog glfw glad stb imgui opengl32)