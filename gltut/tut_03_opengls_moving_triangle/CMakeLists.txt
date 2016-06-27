add_shader_project(standard_vert standard.vert)
add_shader_project(standard_frag standard.frag)
add_shader_project(calc_offset_vert calc_offset.vert)
add_shader_project(position_offset_vert position_offset.vert)
add_shader_project(calc_color_frag calc_color.frag)

add_executable(cpu_position_offset cpu_position_offset.c ../framework/framework.c)
target_link_libraries(cpu_position_offset glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_dependencies(cpu_position_offset standard_vert standard_frag)

add_executable(vert_position_offset vert_position_offset.c ../framework/framework.c)
target_link_libraries(vert_position_offset glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_dependencies(vert_position_offset position_offset_vert standard_frag)

add_executable(vert_calc_offset vert_calc_offset.c ../framework/framework.c)
target_link_libraries(vert_calc_offset glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_dependencies(vert_calc_offset calc_offset_vert standard_frag)

add_executable(frag_change_color frag_change_color.c ../framework/framework.c)
target_link_libraries(frag_change_color glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
add_dependencies(frag_change_color calc_offset_vert calc_color_frag)

