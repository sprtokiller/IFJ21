cmake_minimum_required(VERSION 3.10)

# set the project name
project(IFJ21 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY C_STANDARD 11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "${CMAKE_C_COMPILER_ID} detected, adding compile flags")
if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  # using Clang
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wno-ignored-pragmas")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  # using GCC
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -Werror -g")
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  message(DEPRECATION "Pls don't use pure MSVC")
endif()


configure_file(GTest.cmake
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()
add_compile_definitions(SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_subdirectory("test")
add_subdirectory("gen")

# include header files
include_directories(include)
file(GLOB INCLUDE "include/*.h" "include/*.inl")

# include source files
file(GLOB SOURCES "src/*.c")

# add the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDE})
