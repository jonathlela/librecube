set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/buildtools/cmake/")

find_package(SFML)
find_package(OpenGL)

include_directories(
  ${LibreCube_SOURCE_DIR}/include
)

add_executable(librecube librecube.cpp)

target_link_libraries(librecube
  ${SFML_LIBRARY}
  ${OPENGL_LIBRARY}
)
