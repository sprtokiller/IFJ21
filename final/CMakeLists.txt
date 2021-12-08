cmake_minimum_required(VERSION 3.10)

# set the project name
project(IFJ21final LANGUAGES C)

set_property(GLOBAL PROPERTY C_STANDARD 11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "${CMAKE_C_COMPILER_ID} detected, adding compile flags")
if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  # using Clang
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wno-ignored-pragmas")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  # using GCC
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -g")
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  message(DEPRECATION "Pls don't use pure MSVC")
endif()

add_compile_definitions(SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# include header files
file(GLOB INCLUDE "*.h" "*.inl")

# include source files
file(GLOB SOURCES "*.c")

# add the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDE})
