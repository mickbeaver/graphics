function(add_shader target shader_name)
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name} ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name})
endfunction(add_shader)

add_executable(tut1
  tut_01_hello_triangle/tut1.c
  framework/framework.c)
target_link_libraries(tut1 ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(tut1_exercises
  tut_01_hello_triangle/tut1_exercises.c
  framework/framework.c)
target_link_libraries(tut1_exercises ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(frag_position
  tut_02_playing_with_colors/frag_position.c
  framework/framework.c)
target_link_libraries(frag_position ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_shader(frag_position tut_02_playing_with_colors/frag_position.glsl.vert)
