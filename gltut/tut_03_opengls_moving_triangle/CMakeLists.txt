add_executable(cpu_position_offset cpu_position_offset.c ../framework/framework.c)
target_link_libraries(cpu_position_offset glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_shader_project(cpu_position_offset_shaders standard.vert standard.frag)
add_dependencies(cpu_position_offset cpu_position_offset_shaders)

