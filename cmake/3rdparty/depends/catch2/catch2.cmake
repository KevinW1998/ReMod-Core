include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    catch2
    PREFIX ${CMAKE_BINARY_DIR}/external/catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.2.3
    TIMEOUT 10
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(catch2 source_dir)
message(STATUS "Catch2 source dir is: ${source_dir}")

set(CATCH_INCLUDE_DIR ${source_dir}/single_include CACHE INTERNAL "Path to include folder for Catch")


