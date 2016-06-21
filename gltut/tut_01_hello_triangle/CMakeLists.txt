add_executable(tut1 tut1.c ../framework/framework.c)
target_link_libraries(tut1 glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(tut1_exercises tut1_exercises.c ../framework/framework.c)
target_link_libraries(tut1_exercises glsys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
