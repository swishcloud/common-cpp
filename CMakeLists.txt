cmake_minimum_required(VERSION 3.8)
project(COMMON VERSION 0.1.0)
enable_testing()
set(Boost_USE_STATIC_LIBS on)
set(Boost_DEBUG true)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "COMMON")
#set(CMAKE_SYSTEM_NAME  Android)
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#set(Boost_DIR "/usr/local/lib/cmake/Boost-1.75.0/")
#set(CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\Boost\\lib\\cmake")
#set(Boost_COMPILER mgw8)
#specify output binary name
set(OS_BIT 32)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OS_BIT 64)
endif ()
set(TARGET_NAME COMMON)
set(OUTPUT_NAME ${TARGET_NAME}-${CMAKE_CXX_COMPILER_ID}-${OS_BIT})
string(TOUPPER ${OUTPUT_NAME} OUTPUT_NAME)
set(EXPORT_TARGET_NAME ${OUTPUT_NAME})

if (${CMAKE_SYSTEM_NAME} MATCHES "Android")
set(OPENSSL_ROOT_DIR "/opt/openssl_android")
set(Boost_ROOT_DIR "/opt/boost-android")
list(APPEND CMAKE_FIND_ROOT_PATH "${OPENSSL_ROOT_DIR}" "${Boost_ROOT_DIR}")
else()
add_subdirectory(test)
endif()
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

if (MSVC)
    # warning level 4 and all warnings as errors
    #add_compile_options(/W4 /WX)
else ()
    # lots of warnings and all warnings as errors
    #add_compile_options(-Wall -Wextra -pedantic -Werror)
endif ()

#add library
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_library(${TARGET_NAME} SHARED common.cpp http.cpp linux_monitor.cpp)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Android")
    add_library(${TARGET_NAME} SHARED common.cpp http.cpp)
else ()
    add_library(${TARGET_NAME} STATIC common.cpp http.cpp monitor.cpp)
endif ()
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${OUTPUT_NAME}-$<CONFIG:Debug>)

target_include_directories(${TARGET_NAME} PUBLIC "${OPENSSL_INCLUDE_DIR}" "${Boost_INCLUDE_DIRS}"
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_link_libraries(${TARGET_NAME} PUBLIC OpenSSL::Crypto OpenSSL::SSL)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING REGEX "[^/]*.h$")
install(TARGETS ${TARGET_NAME} EXPORT ${EXPORT_TARGET_NAME}
        LIBRARY DESTINATION lib RENAME XX
        ARCHIVE DESTINATION lib RENAME XX
        RUNTIME DESTINATION bin RENAME XX
        INCLUDES DESTINATION include
        )
set(ConfigPackageLocation lib/cmake/${TARGET_NAME})
configure_file(cmake/${TARGET_NAME}Config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/${TARGET_NAME}Config.cmake"
        COPYONLY
        )
message(---------------------${CMAKE_CXX_COMPILER_ID}-------${TARGET_NAME})
install(EXPORT ${EXPORT_TARGET_NAME} NAMESPACE ${TARGET_NAME}:: DESTINATION ${ConfigPackageLocation})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/${TARGET_NAME}Config.cmake"
"${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/${TARGET_NAME}ConfigVersion.cmake" 
DESTINATION ${ConfigPackageLocation})

message(STATUS ---CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}"$<CONFIG:Debug>"---)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/${TARGET_NAME}ConfigVersion.cmake"  COMPATIBILITY AnyNewerVersion
)