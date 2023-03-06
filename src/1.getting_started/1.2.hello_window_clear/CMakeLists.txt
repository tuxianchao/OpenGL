set(NAME 1.2.hello_window_clear)
file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})