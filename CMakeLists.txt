cmake_minimum_required(VERSION 2.8.12)

project(graphics C)

enable_testing()

if(UNIX)
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)

  if(Clang STREQUAL ${CMAKE_C_COMPILER_ID})
    add_definitions(
      -D_POSIX_C_SOURCE=200809L
      -D_XOPEN_SOURCE=700
      -Werror
      -Weverything
      -Wno-documentation
      -Wno-missing-noreturn
      -Wno-padded
      -pedantic-errors
      -std=c99
      )
  elseif(GNU STREQUAL ${CMAKE_C_COMPILER_ID})
    add_definitions(
      -D_POSIX_C_SOURCE=200809L
      -D_XOPEN_SOURCE=700
      -Wall
      -Werror
      -Wextra
      -pedantic-errors
      -std=c99
      )
  else()
    message(FATAL_ERROR "Unknown compiler. Please add your identifier and flags")
  endif()
elseif(MSVC)
  set(CMAKE_CONFIGURATION_TYPES Debug Release)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  include_directories(${PROJECT_SOURCE_DIR}/external/msvc/include)
  SET(MSVC_EXT_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/external/msvc/lib/${CMAKE_SYSTEM_PROCESSOR}-${MSVC_VERSION})
  if(EXISTS ${MSVC_EXT_LIBRARY_DIR})
    link_directories(${MSVC_EXT_LIBRARY_DIR})
  else()
    message(FATAL_ERROR "No directorty found at ${MSVC_EXT_LIBRARY_DIR} for precompiled libs")
  endif()
endif()

add_subdirectory(3dmath)
add_subdirectory(opengles2)

