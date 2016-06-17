function(add_shader target shader_name)
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name} ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name})
endfunction(add_shader)

add_subdirectory(tut_01_hello_triangle)
add_subdirectory(tut_02_playing_with_colors)