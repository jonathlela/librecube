set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/buildtools/cmake/")

find_package(VAST)
find_package(SFML)
find_package(OpenGL)

include_directories(
  ${LibreCube_SOURCE_DIR}/include
)

file(
  GLOB_RECURSE
  source_files
  *.cpp
  ../include/*.h
)

add_executable(librecube ${source_files})

target_link_libraries(librecube
  ${SFML_LIBRARY}
  ${OPENGL_LIBRARY}
)
