CPPFLAGS := -I3dmath/include
COMMON_CFLAGS := -g -O0 -std=c11 -Wall -Wextra
CFLAGS := $(COMMON_CFLAGS) $(shell sdl2-config --cflags)
LDFLAGS := -lGLESv2 $(shell sdl2-config --libs) -lm
EXECUTABLES := sdl_gl_info sdl_gl_test chapter_04 chapter_06 chapter_06-buffer_objects rotate_triangle tut1 tut1_exercises frag_position
TEST_EXECUTABLES := check_vector check_matrix
TEST_CFLAGS := $(COMMON_CFLAGS)
TEST_LDFLAGS := -lcheck -lm

vpath %.c 3dmath/src 3dmath/tests opengles2 gltut/framework gltut/tut_01_hello_triangle gltut/tut_02_playing_with_colors
vpath %.h 3dmath/include
vpath %.glsl.vert opengles2/shaders gltut/tut_02_playing_with_colors
vpath %.glsl.frag opengles2/shaders gltut/tut_02_playing_with_colors

.PHONY: all clean tests

all: $(EXECUTABLES)

# Objects
matrix.o: scalar.h matrix.h vector.h

vector.o: scalar.h vector.h

# Executables
chapter_04: chapter_04.o | hello_triangle.vert hello_triangle.frag
	$(LINK.c) -o $@ $^ $(LDFLAGS)

chapter_06: chapter_06.o | chapter_06.vert chapter_06.frag
	$(LINK.c) -o $@ $^ $(LDFLAGS)

chapter_06-buffer_objects: chapter_06-buffer_objects.o | chapter_06.vert chapter_06.frag
	$(LINK.c) -o $@ $^ $(LDFLAGS)

rotate_triangle: rotate_triangle.o matrix.o | rotate_triangle.vert rotate_triangle.frag
	$(LINK.c) -o $@ $^ $(LDFLAGS)

tut1: tut1.o framework.o
	$(LINK.c) -o $@ $^ $(LDFLAGS)

tut1_exercises: tut1_exercises.o framework.o
	$(LINK.c) -o $@ $^ $(LDFLAGS)

frag_position: frag_position.o framework.o | frag_position.vert frag_position.frag
	$(LINK.c) -o $@ $^ $(LDFLAGS)

check_vector: CFLAGS = $(TEST_CFLAGS)
check_vector: LDFLAGS = $(TEST_LDFLAGS)
check_vector: check_vector.o vector.o
	$(LINK.c) -o $@ $^ $(TEST_LDFLAGS)

check_matrix: CFLAGS = $(TEST_CFLAGS)
check_matrix: LDFLAGS = $(TEST_LDFLAGS)
check_matrix: check_matrix.o matrix.o vector.o
	$(LINK.c) -o $@ $^ $(TEST_LDFLAGS)

sdl_gl_info: sdl_gl_info.o
	$(LINK.c) -o $@ $^ $(LDFLAGS)

sdl_gl_test: sdl_gl_test.o
	$(LINK.c) -o $@ $^ $(LDFLAGS)

# shaders
# Get glslangValidator from:
#   https://github.com/KhronosGroup/glslang.git
%.vert: %.glsl.vert
	glslangValidator $<
	cp $< $@

%.frag: %.glsl.frag
	glslangValidator $<
	cp $< $@

# Phony targets
tests: $(TEST_EXECUTABLES)
	./check_vector
	./check_matrix

clean:
	-rm -f 	*.o *.vert *.frag $(EXECUTABLES) $(TEST_EXECUTABLES)
