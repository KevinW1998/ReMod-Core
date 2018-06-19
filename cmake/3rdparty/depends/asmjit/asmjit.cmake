include(ExternalProject)
find_package(Git REQUIRED)

set(ASMJIT_INSTALL_DIR ${CMAKE_BINARY_DIR}/external/asmjit-install)
set(ASMJIT_INCLUDE_DIR ${ASMJIT_INSTALL_DIR}/include)
set(ASMJIT_LIBRARY_DIR ${ASMJIT_INSTALL_DIR}/lib)
set(ASMJIT_LIBRARY_FILES ${ASMJIT_LIBRARY_DIR}/asmjit.lib)


ExternalProject_Add(
    asmjit
    PREFIX ${CMAKE_BINARY_DIR}/external/asmjit
    GIT_REPOSITORY https://github.com/asmjit/asmjit.git
    GIT_TAG 673dcefaa048c5f5a2bf8b85daf8f7b9978d018a
    TIMEOUT 10
    LOG_DOWNLOAD ON
	CMAKE_ARGS "-DASMJIT_STATIC=TRUE" "-DASMJIT_BUILD_X86=TRUE" "-DCMAKE_INSTALL_PREFIX=${ASMJIT_INSTALL_DIR}"
)

ExternalProject_Get_Property(asmjit source_dir)
message(STATUS "asmjit source dir is: ${source_dir}")

add_definitions(-DASMJIT_STATIC)

function(asmjit_add_dependency target)
    target_include_directories(${target} PRIVATE ${ASMJIT_INCLUDE_DIR})
    add_dependencies(${target} asmjit)
    target_link_libraries(${target} ${ASMJIT_LIBRARY_FILES})
endfunction()
