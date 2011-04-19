set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/buildtools/cmake/")

find_package(SFML)
find_package(OpenGL)
find_package(VAST)
find_package(ZLIB)

include_directories(
  ${LibreCube_SOURCE_DIR}/include
  ${VAST_INCLUDE_DIR}
)

file(
  GLOB
  source_files
  *.cpp
  ../include/*.h
)

add_definitions("-g")

add_executable(librecube ${source_files})

target_link_libraries(librecube
  ${SFML_LIBRARY}
  ${OPENGL_LIBRARY}
  ${VAST_LIBRARY}
  ${ZLIB_LIBRARY}
)
