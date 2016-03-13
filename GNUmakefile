CPPFLAGS := -I3dmath/include
CFLAGS := -g -O0 -std=c99 -Wall -Wextra	
TEST_EXECUTABLES := check_vector check_matrix
TEST_LDFLAGS := -lcheck -lm

vpath %c 3dmath/src 3dmath/tests
vpath %h 3dmath/include

.PHONY: clean tests

all: $(TEST_EXECUTABLES)

vector.o: scalar.h vector.h

matrix.o: scalar.h matrix.h vector.h

check_vector: check_vector.o vector.o
	$(LINK.c) -o $@ $^ $(TEST_LDFLAGS)

check_matrix: check_matrix.o matrix.o vector.o
	$(LINK.c) -o $@ $^ $(TEST_LDFLAGS)

tests: $(TEST_EXECUTABLES)
	./check_vector
	./check_matrix

clean:
	-rm -f 	*.o $(TEST_EXECUTABLES)
