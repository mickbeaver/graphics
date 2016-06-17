cmake_minimum_required(VERSION 2.8.12)

project(graphics C)

function(add_shader_project)
  list(GET ARGN 0 project_name)
  list(REMOVE_AT ARGN 0)
  foreach(GLSL_FILE ${ARGN})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}
      COMMAND glslangValidator ${CMAKE_CURRENT_SOURCE_DIR}/${GLSL_FILE}
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${GLSL_FILE}
      ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}
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

if(UNIX)
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
else()
  message(FATAL_ERROR "Only UNIX supported.")
endif()

if (armv6l STREQUAL ${CMAKE_HOST_SYSTEM_PROCESSOR})
  message(STATUS "CPU is armv6l. Assuming Raspberry Pi and OpenGL ES 2.")
  set(OPENGL_LIBRARIES GLESv2)
elseif(CYGWIN)
  set(OPENGL_LIBRARIES opengl32)
else()
  set(OPENGL_LIBRARIES GLESv2)
endif()

find_package(PkgConfig)
pkg_check_modules(SDL2_PC REQUIRED sdl2)
add_compile_options(${SDL2_PC_CFLAGS})
set(SDL2_LIBRARIES ${SDL2_PC_LDFLAGS})

add_subdirectory(3dmath)
add_subdirectory(opengles2)
add_subdirectory(gltut)

