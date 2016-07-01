add_shader_project(ortho_with_offset_vert ortho_with_offset.vert)
add_shader_project(standard_colors_frag standard_colors.frag)
add_shader_project(manual_perspective_vert manual_perspective.vert)
add_shader_project(matrix_perspective_vert matrix_perspective.vert)

add_executable(ortho_cube ortho_cube.c ../framework/framework.c)
target_link_libraries(ortho_cube glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} m)
add_dependencies(ortho_cube ortho_with_offset_vert standard_colors_frag)

add_executable(shader_perspective shader_perspective.c ../framework/framework.c)
target_link_libraries(shader_perspective glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} m)
add_dependencies(shader_perspective manual_perspective_vert standard_colors_frag)

add_executable(matrix_perspective matrix_perspective.c ../framework/framework.c)
target_link_libraries(matrix_perspective glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} m)
add_dependencies(matrix_perspective matrix_perspective_vert standard_colors_frag)

