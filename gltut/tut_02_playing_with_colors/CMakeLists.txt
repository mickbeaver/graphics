add_executable(frag_position frag_position.c ../framework/framework.c)
target_link_libraries(frag_position glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_shader_project(frag_position_shaders frag_position.vert frag_position.frag)
add_dependencies(frag_position frag_position_shaders)

add_executable(vertex_colors vertex_colors.c ../framework/framework.c)
target_link_libraries(vertex_colors glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_shader_project(vertex_colors_shaders vertex_colors.vert vertex_colors.frag)
add_dependencies(vertex_colors vertex_colors_shaders)

add_executable(tut2_exercises tut2_exercises.c ../framework/framework.c)
target_link_libraries(tut2_exercises glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_shader_project(tut2_exercises_shaders tut2_exercises.vert tut2_exercises.frag)
add_dependencies(tut2_exercises tut2_exercises_shaders)
