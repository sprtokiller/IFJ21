# include header files
include_directories(include)
file(GLOB INCLUDE "../include/*.h" "include/*.inl")

# include source files
file(GLOB SOURCES "*.cpp")

# add the executable
add_executable("generator" ${SOURCES} ${INCLUDE})

# gen output
add_custom_target( grammar.inl
	COMMAND generator
	OUTPUT "grammar.inl"
	)