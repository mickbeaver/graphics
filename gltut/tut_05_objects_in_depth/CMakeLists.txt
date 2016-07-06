add_shader_project(standard_shaders standard.vert standard.frag)

add_executable(overlap_no_depth overlap_no_depth.c ../framework/framework.c)
target_link_libraries(overlap_no_depth glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} m)
add_dependencies(overlap_no_depth standard_shaders)

add_executable(depth_buffer depth_buffer.c ../framework/framework.c)
target_link_libraries(depth_buffer glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} m)
add_dependencies(depth_buffer standard_shaders)


