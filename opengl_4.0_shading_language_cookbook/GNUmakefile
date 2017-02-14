#vpath %.c external/glew-2.0.0/src
#vpath %.cpp test

CC       := x86_64-w64-mingw32-clang
CXX      := x86_64-w64-mingw32-clang++
CXXFLAGS := -g -O0 -std=c++11
CPPFLAGS := -DGLEW_STATIC -Iexternal/glew-2.0.0/include -Iexternal/glm-0.9.8.4
LDLIBS   := -lopengl32
LINK.o   := $(LINK.cc)
GLEW_OBJ := external/glew-2.0.0/src/glew.o

test/HelloWorld: test/HelloWorld.o $(GLEW_OBJ)

