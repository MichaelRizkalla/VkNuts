################################################################################
### Minimum Cmake required
################################################################################
cmake_minimum_required(VERSION 3.13.0)

################################################################################
### Organize output directories
################################################################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#set(CMAKE_BUILD_TYPE Debug CACHE STRING "")

################################################################################
### Add projects and sub-projects
################################################################################
project(VkNuts)
add_subdirectory(TestRun)
add_subdirectory(VkNutsBase)
add_subdirectory(extern/glfw)
add_subdirectory(extern/spdlog)
