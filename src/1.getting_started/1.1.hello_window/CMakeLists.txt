set(NAME 1.1.hello_window)
file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin/${NAME})
add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})

config_target(${NAME} "1.getting_start")
