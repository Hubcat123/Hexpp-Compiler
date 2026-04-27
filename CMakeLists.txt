cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "Hex++Compiler")
project(${PROJECT_NAME})

set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
src/arena.cpp
src/assembler.cpp
src/generation.cpp
src/main.cpp
src/optimization.cpp 
src/parser.cpp
src/tokenization.cpp 
src/util.cpp
src/hexagonCompat.cpp
)

set(HEADERS 
src/arena.hpp
src/assembler.hpp
src/generation.hpp
src/hexcastingtypes.hpp
src/inbuiltfuncs.hpp
src/optimization.hpp
src/parser.hpp
src/tokenization.hpp
src/util.hpp
src/hexagonCompat.hpp

)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
target_compile_definitions(${PROJECT_NAME} PRIVATE 
$<$<PLATFORM_ID:Windows>:WINDOWS>
$<$<PLATFORM_ID:Linux>:LINUX>
)


if(WIN32)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/hexagon.exe")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/hexagon.exe
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    else()
        message(STATUS "hexagon.exe not found")
endif()

elseif(LINUX)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/hexagon")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/hexagon
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    else()
        message(STATUS "hexagon not found")
    endif()

endif()
 
