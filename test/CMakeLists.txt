

# Enable Testing and Target
enable_testing()
ucm_add_dirs(. TO REMOD_CORE_TEST_SRC RECURSIVE)
add_executable(ReModCoreTest ${REMOD_CORE_TEST_SRC})

# Setup ReMod Dependency
target_link_libraries(ReModCoreTest PRIVATE
    ReModCore::ReModCore
)
set_target_properties(ReModCoreTest PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Setup catch dependencies
target_include_directories(ReModCoreTest PRIVATE ${CATCH_INCLUDE_DIR})
add_dependencies(ReModCoreTest catch2)

# Finally add the test
add_test (NAME "ReModCoreUnitTest" COMMAND ReModCoreTest)





