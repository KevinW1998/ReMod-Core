# == HEADER ==
cmake_minimum_required (VERSION 3.7.2)
project(ReMod-Core)

# == SETTINGS ==
# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")

# Enable multi-processor build
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")


# == INCLUDES ==
include(cmake/3rdparty/ucm.cmake)

# == COLLECT ALL SOURCES ==
ucm_add_dirs(include TO REMOD_CORE_SRC RECURSIVE FILTER_POP 1)
ucm_add_dirs(src TO REMOD_CORE_SRC RECURSIVE FILTER_POP 1)

ucm_add_target(NAME ReModCore TYPE STATIC SOURCES "${REMOD_CORE_SRC}")

target_include_directories(ReModCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# == INSTALL ==
install(TARGETS ReModCore
    ARCHIVE DESTINATION "lib"
    LIBRARY DESTINATION "bin"
    RUNTIME DESTINATION "bin")

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/remod.h" DESTINATION "include")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/remod" DESTINATION "include")

