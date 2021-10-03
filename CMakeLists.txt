cmake_minimum_required(VERSION 3.10)

# set the project name
project(IFJ21 C)


if (CMAKE_C_COMPILER_VERSION VERSION_LESS 8.0)
    message(FATAL_ERROR "GCC version must be at least 8.0!")
endif()

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -Werror -g")

# include header files
include_directories(include)

# include source files
file(GLOB SOURCES "src/*.c")

# add the executable
add_executable(outputFile ${SOURCES})