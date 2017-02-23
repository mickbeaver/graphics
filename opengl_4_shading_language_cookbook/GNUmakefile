CC          := x86_64-w64-mingw32-clang
CXX         := x86_64-w64-mingw32-clang++
SDL2_CFLAGS := $(shell x86_64-w64-mingw32-pkg-config sdl2 --cflags)
SDL2_LIBS   := $(shell x86_64-w64-mingw32-pkg-config sdl2 --libs)
CXXFLAGS    := -g -O0 -std=c++11 -Wall -Werror $(SDL2_CFLAGS)
CPPFLAGS    := -Iglsys/include -Iexternal/glm-0.9.8.4
GLSYS_O     := glsys/src/glsys.o
LDLIBS      := -lopengl32 $(SDL2_LIBS)
LINK.o      := $(LINK.cc)

EXECUTABLES := test/HelloWorld
EXECUTABLES := $(EXECUTABLES) 010-using_function_loader/TestFunctionLoader
EXECUTABLES := $(EXECUTABLES) 010-using_glm/TestGlm
EXECUTABLES := $(EXECUTABLES) 013-determining_version/DetermineVersion
EXECUTABLES := $(EXECUTABLES) 015-compiling_a_shader/CompilingAShader
EXECUTABLES := $(EXECUTABLES) 018-linking_a_shader/LinkingAShader
EXECUTABLES := $(EXECUTABLES) 022-sending_data/SendingData
EXECUTABLES := $(EXECUTABLES) 029-active_attributes/ListActiveAttributes

.PHONY: all clean

all: $(EXECUTABLES)

clean:
	find . -name "*.o" -type f -delete
	rm -f $(EXECUTABLES)

test/HelloWorld: test/HelloWorld.o $(GLSYS_O)

010-using_function_loader/TestFunctionLoader: 010-using_function_loader/TestFunctionLoader.o $(GLSYS_O)

010-using_glm/TestGlm: 010-using_glm/TestGlm.o $(GLSYS_O)

013-determining_version/DetermineVersion: 013-determining_version/DetermineVersion.o $(GLSYS_O)

015-compiling_a_shader/CompilingAShader: 015-compiling_a_shader/CompilingAShader.o 015-compiling_a_shader/FileUtil.o $(GLSYS_O)

018-linking_a_shader/LinkingAShader: 018-linking_a_shader/LinkingAShader.o 018-linking_a_shader/FileUtil.o $(GLSYS_O)

022-sending_data/SendingData: 022-sending_data/SendingData.o 022-sending_data/FileUtil.o $(GLSYS_O)

029-active_attributes/ListActiveAttributes: 029-active_attributes/ListActiveAttributes.o 029-active_attributes/FileUtil.o $(GLSYS_O)
