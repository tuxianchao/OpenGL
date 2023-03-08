set(NAME 1.2.hello_window_clear)
file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin/${NAME})
add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set_property(TARGET ${NAME} PROPERTY  FOLDER "1")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
    set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${BIN_DIR}/${CMAKE_BUILD_TYPE}")
elseif(APPLE)
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}/immediate/Debug")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}/immediate/Release")
endif(WIN32)
