# Credit to http://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/
# for the awesome tutorial on modern cmake

# Collect the sources
ucm_add_dirs(../include TO REMOD_CORE_SRC RECURSIVE FILTER_POP 2)  # For Visual Studio
ucm_add_dirs(remod TO REMOD_CORE_SRC RECURSIVE FILTER_POP 0)

# Setup Target
add_library(ReModCore ${REMOD_CORE_SRC})
add_library(ReModCore::ReModCore ALIAS ReModCore)

# Setup dependencies
asmjit_add_dependency(ReModCore)

# Setup Static and Shared Switch
option(BUILD_SHARED_LIBS "Build shared library" ON)
include(GenerateExportHeader)
generate_export_header(ReModCore
    BASE_NAME REMOD_CORE
    EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/../include/remod/common.h
)

# Setup Includes
target_include_directories(ReModCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Setup C++17 and compile options
target_compile_features(ReModCore PUBLIC cxx_std_17)
target_compile_options(ReModCore PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -pipe -march=native
        $<$<CONFIG:Debug>:-O0 -Wall -Wextra>>
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:/Od /W4 /Zi /wd4514>>
)

# TODO: Set additional compiler flags like /MP
# TODO: Cleanup the parameters up there

set_target_properties(ReModCore PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)



include(GNUInstallDirs)

install(TARGETS ReModCore
    EXPORT ReModCore-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY ../include/remod
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ReModCore-targets
    FILE ReModCore-targets.cmake
    NAMESPACE ReModCore::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ReModCore
)

# TODO: Setup the rest of the config stuff...









