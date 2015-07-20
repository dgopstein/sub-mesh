INCLUDE (ExternalProject)

cmake_minimum_required (VERSION 2.6)
project (sub_mesh)
set (targetName sub_mesh)

message("\nPROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}\n")

### GLFW

SET(GLFW_PATH $ENV{HOME}/opt/lib/glfw-3.1.1 glfw-dir)
add_subdirectory(${GLFW_PATH})
include_directories(
  external/glfw-3.1.1/include/GLFW/
  external/glm/
)

### OpenMesh

include_directories ( ${PROJECT_SOURCE_DIR}/external/OpenMesh-3.3/cmake/) # Not necessary?
  
add_subdirectory(${PROJECT_SOURCE_DIR}/external/OpenMesh-3.3) # This line builds OpenMesh


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/external/OpenMesh-3.3/cmake/)

include (ACGCommon)
#include (external/OpenMesh-3.3/cmake/ACGCommon.cmake)

# collect all header and source files
acg_append_files (headers "*.hh" .)
acg_append_files (sources "*.cc" .)

### Build sub_mesh

#acg_add_executable (${targetName} ${headers} ${sources})
FILE(GLOB CommonSource common/*.cpp)
acg_add_executable (${targetName} main.cpp ${CommonSource})

### Link libraries

target_link_libraries(${targetName} OpenMeshCore OpenMeshTools)
target_link_libraries(${targetName} glfw ${GLFW_LIBRARIES})
target_link_libraries(${targetName} glew ${GLEW_LIBRARIES})

install(TARGETS ${targetName} DESTINATION bin)

### Move shaders to output dir

file(GLOB VERTEX_SHADERS "shaders/*.vert")
file(GLOB FRAGMENT_SHADERS "shaders/*.frag")
file(COPY ${VERTEX_SHADERS} DESTINATION .)
file(COPY ${FRAGMENT_SHADERS} DESTINATION .)
