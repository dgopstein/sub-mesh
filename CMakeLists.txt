INCLUDE (ExternalProject)

cmake_minimum_required (VERSION 2.6)
project (sub_mesh)


SET(GLFW_PATH $ENV{HOME}/opt/lib/glfw-3.1.1 glfw-dir)
add_subdirectory(${GLFW_PATH})
#include_directories(${CMAKE_SOURCE_DIR}/glfw-3.1.1/include)
include_directories(
  external/glfw-3.1.1/include/GLFW/
  external/glm/
)

FILE(GLOB CommonSource common/*.cpp)

add_executable(sub_mesh main.cpp ${CommonSource})

#set(ALL_LIBS
#  ${OPENGL_LIBRARY}
#  GLFW_311
#  GLEW_190
#)

target_link_libraries(sub_mesh glfw ${GLFW_LIBRARIES})
target_link_libraries(sub_mesh glew ${GLEW_LIBRARIES})
#target_link_libraries(sub_mesh glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})

#SET (SUBDIRS extern)
#FOREACH (sbdir ${SUBDIRS})
#    ADD_SUBDIRECTORY (${sbdir})
#ENDFOREACH (sbdir)


#ExternalProject_Add(project_glfw
#  URL https://github.com/glfw/glfw/archive/3.1.1.tar.gz
#  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glfw-3.1.1
#)

#include_directories(glfw-3.1.1/src/project_glfw/include/)
#find_package(glfw REQUIRED)

#set_property(TARGET glfw PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libluajit-5.1.a)
#add_dependencies(glfw project_glfw)
#add_library(project_glfw STATIC IMPORTED)


install(TARGETS sub_mesh DESTINATION bin)
