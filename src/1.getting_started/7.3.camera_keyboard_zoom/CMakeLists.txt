set(NAME 7.3.camera_keyboard_zoom)
file(GLOB SOURCE
    "*.h"
    "*.cpp"
    "*.vs"
    "*.fs"
)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin/${NAME})
add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})

config_target(${NAME} "1.getting_start")
