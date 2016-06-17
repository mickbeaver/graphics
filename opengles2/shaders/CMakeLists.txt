file(GLOB GLSL_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.vert *.frag *.glsl *.inc)
foreach(GLSL_FILE ${GLSL_SOURCE_FILES})
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${GLSL_FILE}
                                                      ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE}
                     DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${GLSL_FILE}")
  list(APPEND GLSL_DEST_FILES ${CMAKE_CURRENT_BINARY_DIR}/${GLSL_FILE})
endforeach()
add_custom_target(shaders ALL
                  SOURCES ${GLSL_SOURCE_FILES}
                  DEPENDS ${GLSL_DEST_FILES}) 
source_group(Shader\ Source\ Files FILES ${GLSL_SOURCE_FILES})
