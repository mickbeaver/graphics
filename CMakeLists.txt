cmake_minimum_required(VERSION 2.8.12)

project(graphics C)

if(UNIX)
  if(Clang STREQUAL ${CMAKE_C_COMPILER_ID})
    add_definitions(-Werror -Weverything -Wno-padded -Wno-documentation -std=c99 -pedantic-errors)
  elseif(GNU STREQUAL ${CMAKE_C_COMPILER_ID})
    add_definitions(-Werror -Wall -Wextra -pedantic-errors -std=c99)
  else()
    message(FATAL_ERROR "Unknown compiler. Please add your identifier and flags")
  endif()
else()
  message(FATAL_ERROR "Only Unix systems are supported.")
endif()

add_subdirectory(3dmath)
add_subdirectory(opengles2)

