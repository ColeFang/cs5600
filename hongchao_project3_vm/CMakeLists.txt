cmake_minimum_required(VERSION 3.14)
project(VirtualMemFrameworkC C)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    execute_process(COMMAND dpkg -l git build-essential cmake
            RESULT_VARIABLE result
            OUTPUT_QUIET ERROR_QUIET)
    if(result)
        message(
                FATAL_ERROR
                "Missing dependencies. Fix by running this command:\nsudo apt -y install git build-essential cmake"
        )
    endif()
endif()

configure_file(CMakeLists.txt.in ext/unity/download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/unity/download )
if(result)
    message(FATAL_ERROR "CMake step for unity failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/unity/download )
if(result)
    message(FATAL_ERROR "Build step for unity failed: ${result}")
endif()


set(CMAKE_C_STANDARD 11)

add_library(unity STATIC ${CMAKE_BINARY_DIR}/ext/unity/unity/src/unity.c)

include_directories(${CMAKE_BINARY_DIR}/ext/unity/unity/src src src/tests src/answer)
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
add_executable(VirtualMemFrameworkC ${SOURCES})
target_include_directories(unity PUBLIC ${CMAKE_BINARY_DIR}/ext/unity/unity/src src)
target_link_libraries(VirtualMemFrameworkC pthread unity)