

# Enable Testing and Target
enable_testing()
ucm_add_dirs(. TO REMOD_CORE_TEST_SRC RECURSIVE)
add_executable(ReModCoreTest ${REMOD_CORE_TEST_SRC})

# Setup ReMod Dependency
target_link_libraries(ReModCoreTest PRIVATE
    ReModCore::ReModCore
)
add_custom_command(TARGET ReModCoreTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} --verbose -E copy_if_different
        "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/ReModCore.dll"
        $<TARGET_FILE_DIR:ReModCoreTest>)
		

# Setup catch dependencies
target_include_directories(ReModCoreTest PRIVATE ${CATCH_INCLUDE_DIR})
add_dependencies(ReModCoreTest catch2)

# Finally add the test
add_test (NAME "ReModCoreUnitTest" COMMAND ReModCoreTest)





