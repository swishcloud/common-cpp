cmake_minimum_required(VERSION 3.0.0)
project(common VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "common")

include(CTest)
enable_testing()

add_library(common STATIC common.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}\\include")
include(GNUInstallDirs)
install(TARGETS common
LIBRARY DESTINATION lib
)
install(DIRECTORY include/ TYPE INCLUDE  FILES_MATCHING REGEX  "[^/]*.h$")