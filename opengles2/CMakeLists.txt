add_subdirectory(shaders)
include_directories(../3dmath/include)

add_executable(sdl_gl_info sdl_gl_info.c ${PLATFORM_SRCS})
target_link_libraries(sdl_gl_info 3dmath ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(sdl_gl_test sdl_gl_test.c ${PLATFORM_SRCS})
target_link_libraries(sdl_gl_test ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(chapter_04 chapter_04.c ${PLATFORM_SRCS})
add_dependencies(chapter_04 opengles2_shaders)
target_link_libraries(chapter_04 ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(chapter_06 chapter_06.c ${PLATFORM_SRCS})
add_dependencies(chapter_06 opengles2_shaders)
target_link_libraries(chapter_06 ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(chapter_06-buffer_objects chapter_06-buffer_objects.c ${PLATFORM_SRCS})
add_dependencies(chapter_06-buffer_objects opengles2_shaders)
target_link_libraries(chapter_06-buffer_objects ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(rotate_triangle rotate_triangle.c ${PLATFORM_SRCS})
add_dependencies(rotate_triangle opengles2_shaders)
target_link_libraries(rotate_triangle 3dmath ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

