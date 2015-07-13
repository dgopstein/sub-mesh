INCLUDE (ExternalProject)

cmake_minimum_required (VERSION 2.6)
project (sub_mesh)


#SET (SUBDIRS extern)
#FOREACH (sbdir ${SUBDIRS})
#    ADD_SUBDIRECTORY (${sbdir})
#ENDFOREACH (sbdir)


ExternalProject_Add(project_glfw
  URL https://github.com/glfw/glfw/archive/3.0.1.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glfw-3.0.1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make
  INSTALL_COMMAND make install PREFIX=${CMAKE_CURRENT_BINARY_DIR}/glfw-3.0.1
)

ExternalProject_Get_Property(project_glfw install_dir)

add_library(glfw STATIC IMPORTED)
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libglfw-5.1.a)
add_dependencies(glfw project_glfw)


add_executable(sub_mesh main.cpp)
include_directories(${install_dir}/include/glfw-3.0)
target_link_libraries(sub_mesh glfw)

install(TARGETS sub_mesh DESTINATION bin)
