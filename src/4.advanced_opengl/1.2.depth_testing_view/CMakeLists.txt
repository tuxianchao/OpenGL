set(NAME 1.2.depth_testing_view)
file(GLOB SOURCE
    "*.h"
    "*.cpp"
    "*.vs"
    "*.fs"
)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin/${NAME})
add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})

config_target(${NAME} "4.advanced_opengl")
