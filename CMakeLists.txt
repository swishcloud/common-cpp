cmake_minimum_required(VERSION 3.0.0)
project(common VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "common")
add_library(common STATIC common.cpp)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
target_include_directories(common PRIVATE include)
include(GNUInstallDirs)
install(DIRECTORY include/ TYPE INCLUDE  FILES_MATCHING REGEX  "[^/]*.h$")
#TARGET_INCLUDE_DIRECTORIES(common PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
#install(FILES "include/common.h" DESTINATION include)
install(TARGETS common EXPORT commonTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
set(ConfigPackageLocation lib/cmake/common)
configure_file(cmake/commonConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/common/commonConfig.cmake"
  COPYONLY
)
install(EXPORT commonTargets NAMESPACE common:: DESTINATION  ${ConfigPackageLocation})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/common/commonConfig.cmake" DESTINATION  ${ConfigPackageLocation})