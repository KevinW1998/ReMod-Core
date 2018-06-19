# == HEADER ==
cmake_minimum_required(VERSION 3.10)
project(ReMod-Core
	LANGUAGES CXX
	VERSION 0.1.0)

# == INCLUDES ==
include(cmake/3rdparty/ucm.cmake)
include(cmake/3rdparty/depends/asmjit/asmjit.cmake)
include(cmake/3rdparty/depends/catch2/catch2.cmake)

# == COLLECT ALL SOURCES ==
add_subdirectory(src)
add_subdirectory(test)



