set(NAME 3.6.shaders_exercise3)
file(GLOB SOURCE
    "*.h"
    "*.cpp"
    "*.vs"
    "*.fs"
)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin/${NAME})
add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set_property(TARGET ${NAME} PROPERTY  FOLDER "1.getting_started")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
    set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${BIN_DIR}/${CMAKE_BUILD_TYPE}")
elseif(APPLE)
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}/immediate/Debug")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}/immediate/Release")
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
    message(copy ${SHADER})
    if(WIN32)
        # configure_file(${SHADER} "test")
        add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
    elseif(APPLE)
        # create symbolic link for *.vs *.fs *.gs
        get_filename_component(SHADERNAME ${SHADER} NAME)
        makeLink(${SHADER} ${BIN_DIR}/${SHADERNAME} ${NAME})
    endif(WIN32)
endforeach(SHADER)


