add_compile_options(${SDL2_PC_CFLAGS})
add_subdirectory(shaders)

add_executable(sdl_gl_info sdl_gl_info.c)
target_link_libraries(sdl_gl_info glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(sdl_gl_test sdl_gl_test.c ${PLATFORM_SRCS})
target_link_libraries(sdl_gl_test glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(chapter_04 chapter_04.c ${PLATFORM_SRCS})
add_dependencies(chapter_04 opengles2_shaders)
target_link_libraries(chapter_04 glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(chapter_06 chapter_06.c ${PLATFORM_SRCS})
add_dependencies(chapter_06 opengles2_shaders)
target_link_libraries(chapter_06 glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(chapter_06-buffer_objects chapter_06-buffer_objects.c ${PLATFORM_SRCS})
add_dependencies(chapter_06-buffer_objects opengles2_shaders)
target_link_libraries(chapter_06-buffer_objects glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(rotate_triangle rotate_triangle.c ${PLATFORM_SRCS})
add_dependencies(rotate_triangle opengles2_shaders)
target_link_libraries(rotate_triangle 3dmath glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

