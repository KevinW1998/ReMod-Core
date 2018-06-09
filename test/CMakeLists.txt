
enable_testing()

ucm_add_dirs(. TO REMOD_CORE_TEST_SRC RECURSIVE)

add_executable(ReModCoreTest ${REMOD_CORE_TEST_SRC})

target_link_libraries(ReModCoreTest PRIVATE
    ReModCore::ReModCore
)

# Add catch dependencies
target_include_directories(ReModCoreTest PRIVATE ${CATCH_INCLUDE_DIR})
add_dependencies(ReModCoreTest catch2)

add_test (NAME "ReModCoreUnitTest" COMMAND ReModCoreTest)





