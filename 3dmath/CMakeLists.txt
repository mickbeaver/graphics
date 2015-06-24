include_directories(include)

set(math_srcs
    include/vector.h
    include/matrix.h
    src/vector.c
    src/matrix.c)

add_library(3dmath ${math_srcs})

