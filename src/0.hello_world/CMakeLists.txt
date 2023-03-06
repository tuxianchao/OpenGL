set(NAME hello_world)
file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

add_executable(${NAME} ${SOURCE})