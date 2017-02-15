#vpath %.c external/glew-2.0.0/src
#vpath %.cpp test

CC          := x86_64-w64-mingw32-clang
CXX         := x86_64-w64-mingw32-clang++
SDL2_CFLAGS := $(shell x86_64-w64-mingw32-pkg-config sdl2 --cflags)
SDL2_LIBS   := $(shell x86_64-w64-mingw32-pkg-config sdl2 --libs)
CXXFLAGS    := -g -O0 -std=c++11 -Wall $(SDL2_CFLAGS)
CPPFLAGS    := -DGLEW_STATIC -Iexternal/glew-2.0.0/include -Iexternal/glm-0.9.8.4
LDLIBS      := -lopengl32 $(SDL2_LIBS)
LINK.o      := $(LINK.cc)
GLEW_O      := external/glew-2.0.0/src/glew.o
EXECUTABLES := test/HelloWorld
EXECUTABLES := $(EXECUTABLES) 008-using_glew/TestGlew
EXECUTABLES := $(EXECUTABLES) 010-using_glm/TestGlm

.PHONY: all clean

all: $(EXECUTABLES)

clean:
	find . -name "*.o" -type f -delete
	rm -f $(EXECUTABLES)

test/HelloWorld: test/HelloWorld.o $(GLEW_O)

008-using_glew/TestGlew: 008-using_glew/TestGlew.o $(GLEW_O)

010-using_glm/TestGlm: 010-using_glm/TestGlm.o $(GLEW_O)
