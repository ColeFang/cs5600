cmake_minimum_required(VERSION 3.17)
project(FileSystem C)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    execute_process(COMMAND dpkg -l git uuid-dev build-essential cmake
            RESULT_VARIABLE result
            OUTPUT_QUIET ERROR_QUIET)
    if(result)
        message(
                FATAL_ERROR
                "Missing dependencies. Fix by running this command:\nsudo apt -y install git uuid-dev build-essential cmake"
        )
    endif()
endif()


configure_file(CMakeLists.txt.in ext/Unity/download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/Unity/download)
if(result)
    message(FATAL_ERROR "CMake step for Unity failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/Unity/download)
if(result)
    message(FATAL_ERROR "Build step for Unity failed: ${result}")
endif()

set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(${CMAKE_BINARY_DIR}/ext/Unity/src
        ${CMAKE_BINARY_DIR}/ext/Unity/build EXCLUDE_FROM_ALL)

set(CMAKE_C_STANDARD 99)

include_directories(. ./src ${CMAKE_BINARY_DIR}/ext/Unity/src/src)

add_executable(FileSystem src/main.c src/answer/disk.c src/tests/diskTests.c src/tests/fileTests.c src/bitmap.c src/tests/dirTests.c src/tests/fileTests.c)
target_link_libraries(FileSystem m unity)
