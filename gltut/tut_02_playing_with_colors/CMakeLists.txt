add_executable(frag_position frag_position.c ../framework/framework.c)
target_link_libraries(frag_position ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_shader_project(frag_position_shaders frag_position.vert frag_position.frag)
add_dependencies(frag_position frag_position_shaders)

