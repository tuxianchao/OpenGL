set(NAME spd_log_test)
file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})