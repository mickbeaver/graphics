include_directories(include)

set(math_srcs
    include/vector.h
    include/matrix.h
    src/vector.c
    src/matrix.c)

add_library(3dmath ${math_srcs})
target_link_libraries(3dmath m)

add_executable(check_vector tests/check_vector.c)
target_link_libraries(check_vector 3dmath check)
add_test(check_vector check_vector)

add_executable(check_matrix tests/check_matrix.c)
target_link_libraries(check_matrix 3dmath check m)
add_test(check_matrix check_matrix)
