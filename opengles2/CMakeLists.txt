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

add_subdirectory(shaders)
include_directories(../3dmath/include)

add_executable(sdl_gl_info sdl_gl_info.c ${PLATFORM_SRCS})
target_link_libraries(sdl_gl_info 3dmath ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(sdl_gl_test sdl_gl_test.c ${PLATFORM_SRCS})
target_link_libraries(sdl_gl_test ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(chapter_04 chapter_04.c ${PLATFORM_SRCS})
add_dependencies(chapter_04 shaders)
target_link_libraries(chapter_04 ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(chapter_06 chapter_06.c ${PLATFORM_SRCS})
add_dependencies(chapter_06 shaders)
target_link_libraries(chapter_06 ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(chapter_06-buffer_objects chapter_06-buffer_objects.c ${PLATFORM_SRCS})
add_dependencies(chapter_06-buffer_objects shaders)
target_link_libraries(chapter_06-buffer_objects ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

