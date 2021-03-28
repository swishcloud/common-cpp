cmake_minimum_required(VERSION 3.0.0)
project(common VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "common")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_PREFIX_PATH "/opt/openssl")
find_package(openssl REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time )
#add library
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
add_library(common STATIC common.cpp http.cpp linux_monitor.cpp)
else()
add_library(common STATIC common.cpp http.cpp monitor.cpp)
endif()
target_include_directories(common PRIVATE include "${OPENSSL_INCLUDE_DIR}/include" "${Boost_INCLUDE_DIRS}")
message(OPENSSL HEADER DIECTORY:${OPENSSL_INCLUDE_DIR}/include/openssl)
install(DIRECTORY include/ TYPE INCLUDE  FILES_MATCHING REGEX  "[^/]*.h$")
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
