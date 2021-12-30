set(OS_BIT 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(OS_BIT 64)
endif()
set(TARGET_PATH "COMMON-${CMAKE_CXX_COMPILER_ID}-${OS_BIT}")
string(TOUPPER ${TARGET_PATH} TARGET_PATH)
set(TARGET_PATH "${CMAKE_CURRENT_LIST_DIR}/${TARGET_PATH}.cmake")
message(STATUS ${TARGET_PATH})
include(${TARGET_PATH})