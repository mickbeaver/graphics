set(math_library m)

set(math_srcs
    include/matrix.h
    include/scalar.h
    include/vector.h
    src/matrix.c
    src/scalar.c
    src/vector.c)

add_library(3dmath ${math_srcs})
target_link_libraries(3dmath ${math_library})

add_executable(check_vector tests/check_vector.c tests/greatest.h)
target_link_libraries(check_vector 3dmath ${math_library})
add_test(check_vector check_vector)

add_executable(check_matrix tests/check_matrix.c tests/greatest.h)
target_link_libraries(check_matrix 3dmath ${math_library})
add_test(check_matrix check_matrix)

