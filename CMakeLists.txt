cmake_minimum_required(VERSION 2.8.12)

project(graphics C)

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

add_subdirectory(3dmath)
add_subdirectory(opengles2)

