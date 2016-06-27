cmake_minimum_required(VERSION 2.8.12)

project(graphics C)

function(add_shader_project)
  list(GET ARGN 0 project_name)
  list(REMOVE_AT ARGN 0)
  foreach(GLSL_FILE ${ARGN})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}.pending
      COMMAND echo \"${GLSL_VERSION_STRING}\" > ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}.pending/${GLSL_FILE}
      COMMAND echo \"${GLSL_DEFINE_VS_IN}\" >> ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}.pending/${GLSL_FILE}
      COMMAND echo \"${GLSL_DEFINE_VS_OUT}\" >> ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}.pending/${GLSL_FILE}
      COMMAND echo \"\" >> ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}.pending/${GLSL_FILE}
      COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/${GLSL_FILE} >> ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}.pending/${GLSL_FILE}
      COMMAND glslangValidator ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}.pending/${GLSL_FILE}
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}.pending/${GLSL_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}.pending
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${GLSL_FILE}")
    list(APPEND GLSL_DEST_FILES ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE})
  endforeach()
  message(STATUS ${project_name})
  add_custom_target(${project_name} ALL
    SOURCES ${GLSL_SOURCE_FILES}
    DEPENDS ${GLSL_DEST_FILES}) 
  source_group("Shader Source Files" FILES ${GLSL_SOURCE_FILES})
endfunction(add_shader_project)

enable_testing()

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

if(Clang STREQUAL ${CMAKE_C_COMPILER_ID})
  add_definitions(
    -Werror
    -Weverything
    -Wno-documentation
    -Wno-missing-noreturn
    -Wno-padded
    -pedantic-errors
    -std=c11
    )
elseif(GNU STREQUAL ${CMAKE_C_COMPILER_ID})
  add_definitions(
    -Wall
    -Werror
    -Wextra
    -pedantic-errors
    -std=c11
    )
else()
  message(FATAL_ERROR "Unknown compiler. Please add your identifier and flags")
endif()

find_library(OPENGL_LIBRARIES NAMES GLESv2 GL opengl32)
if(${OPENGL_LIBRARIES} MATCHES GLESv2)
  message(STATUS "Using OpenGL ES 2 library.")
  add_definitions(-DGL_ES)
  # Space between tokens for CMake
  set(GLSL_VERSION_STRING "#version" "100")
  set(GLSL_DEFINE_VS_IN      "#define" "VS_IN" "attribute")
  set(GLSL_DEFINE_VS_OUT     "#define" "VS_OUT" "varying")
else()
  message(STATUS "Using desktop OpenGL (3.2 core) library.")
  # Space between tokens for CMake
  set(GLSL_VERSION_STRING "#version" "150")
  set(GLSL_DEFINE_VS_IN      "#define" "VS_IN" "in")
  set(GLSL_DEFINE_VS_OUT     "#define" "VS_OUT" "out")
endif()
if(NOT OPENGL_LIBRARIES)
  message(FATAL_ERROR "No usuable OpenGL library found.")
endif()

find_package(PkgConfig)
pkg_check_modules(SDL2_PC REQUIRED sdl2)
set(SDL2_LIBRARIES ${SDL2_PC_LDFLAGS})

include_directories(glsys/include 3dmath/include)

add_subdirectory(3dmath)
add_subdirectory(glsys)
add_subdirectory(opengles2)
add_subdirectory(gltut)

