cmake_minimum_required (VERSION 2.6)
project (aspen)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set (aspen_VERSION_MAJOR 1)
set (aspen_VERSION_MINOR 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/BuildConfig.h.in"
  "${PROJECT_BINARY_DIR}/BuildConfig.h")
include_directories("${PROJECT_BINARY_DIR}"
${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_subdirectory(xml)
add_subdirectory(angelscript)
add_subdirectory(sqlite3cpp)
add_subdirectory(src)
