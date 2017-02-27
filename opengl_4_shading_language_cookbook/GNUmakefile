CC           := x86_64-w64-mingw32-gcc
CXX          := x86_64-w64-mingw32-g++
SDL2_CFLAGS  := $(shell x86_64-w64-mingw32-pkg-config sdl2 --cflags)
SDL2_LIBS    := $(shell x86_64-w64-mingw32-pkg-config sdl2 --libs)
SHARED_FLAGS := -g -O0 -Wall -Werror -pedantic-errors $(SDL2_CFLAGS)
CFLAGS       := $(SHARED_FLAGS) -std=c11
CXXFLAGS     := $(SHARED_FLAGS) -std=c++11
CPPFLAGS     := -DDEBUG -Iglsys/include -Iexternal/glm-0.9.8.4
GLSYS_O      := glsys/src/glsys.o
LDLIBS       := -lopengl32 $(SDL2_LIBS)
LINK.o       := $(LINK.cc)

EXECUTABLES := test/HelloWorld
EXECUTABLES := $(EXECUTABLES) 010-using_function_loader/TestFunctionLoader
EXECUTABLES := $(EXECUTABLES) 013-using_glm/TestGlm
EXECUTABLES := $(EXECUTABLES) 015-determining_version/DetermineVersion
EXECUTABLES := $(EXECUTABLES) 017-compiling_a_shader/CompilingAShader
EXECUTABLES := $(EXECUTABLES) 021-linking_a_shader/LinkingAShader
EXECUTABLES := $(EXECUTABLES) 025-sending_data/SendingData
EXECUTABLES := $(EXECUTABLES) 034-active_attributes/ListActiveAttributes
EXECUTABLES := $(EXECUTABLES) 037-using_uniforms/UsingUniforms

.PHONY: all clean

all: $(EXECUTABLES)

clean:
	find . -name "*.o" -type f -delete
	rm -f $(EXECUTABLES)

test/HelloWorld: test/HelloWorld.o $(GLSYS_O)

010-using_function_loader/TestFunctionLoader: 010-using_function_loader/TestFunctionLoader.o $(GLSYS_O)

013-using_glm/TestGlm: 013-using_glm/TestGlm.o $(GLSYS_O)

015-determining_version/DetermineVersion: 015-determining_version/DetermineVersion.o $(GLSYS_O)

017-compiling_a_shader/CompilingAShader: 017-compiling_a_shader/CompilingAShader.o 017-compiling_a_shader/FileUtil.o $(GLSYS_O)

021-linking_a_shader/LinkingAShader: 021-linking_a_shader/LinkingAShader.o 021-linking_a_shader/FileUtil.o $(GLSYS_O)

025-sending_data/SendingData: 025-sending_data/SendingData.o 025-sending_data/FileUtil.o $(GLSYS_O)

034-active_attributes/ListActiveAttributes: 034-active_attributes/ListActiveAttributes.o 034-active_attributes/FileUtil.o $(GLSYS_O)

037-using_uniforms/UsingUniforms: 037-using_uniforms/UsingUniforms.o 037-using_uniforms/FileUtil.o $(GLSYS_O)
