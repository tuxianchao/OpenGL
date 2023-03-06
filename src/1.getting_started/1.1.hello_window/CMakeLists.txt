set(NAME 1.1.hello_window)
file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})