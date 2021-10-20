cmake_minimum_required(VERSION 3.10)

# set the project name
project(IFJ21 C)
set_property(GLOBAL PROPERTY C_STANDARD 11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "${CMAKE_C_COMPILER_ID} detected, adding compile flags")
if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  # using Clang
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  # using GCC
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -Werror -g")
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  message(DEPRECATION "Pls don't use pure MSVC")
endif()


# include header files
include_directories(include)
file(GLOB INCLUDE "include/*.h")

# include source files
file(GLOB SOURCES "src/*.c")

# add the executable
add_executable(ifj21compiler ${SOURCES} ${INCLUDE})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ifj21compiler)
