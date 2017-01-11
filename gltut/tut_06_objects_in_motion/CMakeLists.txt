add_shader_project(pos_color_local_transform_vert pos_color_local_transform.vert)
add_shader_project(color_passthrough_frag color_passthrough.frag)

add_executable(translation translation.c ../framework/framework.c)
target_link_libraries(translation glsys 3dmath ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} m)
add_dependencies(translation pos_color_local_transform_vert color_passthrough_frag)

add_executable(scale scale.c ../framework/framework.c)
target_link_libraries(scale glsys 3dmath ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} m)
add_dependencies(scale pos_color_local_transform_vert color_passthrough_frag)

add_executable(rotations rotations.c ../framework/framework.c)
target_link_libraries(rotations glsys 3dmath ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} m)
add_dependencies(rotations pos_color_local_transform_vert color_passthrough_frag)

