cmake_minimum_required(VERSION 2.6)
project(HelloWorld)

set(CMAKE_MODULE_PATH "../../buildtools/cmake/")

find_package(VAST)
find_package(ZLIB)

include_directories(
  ${VAST_INCLUDE_DIR}
)

add_definitions("-g")

add_executable(vast hw-vast.cpp)

target_link_libraries(vast
  ${VAST_LIBRARY}
  ${ZLIB_LIBRARY}
)

