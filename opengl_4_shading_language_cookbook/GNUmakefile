AR           := x86_64-w64-mingw32-ar
CC           := x86_64-w64-mingw32-gcc
CXX          := x86_64-w64-mingw32-g++
SDL2_CFLAGS  := $(shell x86_64-w64-mingw32-pkg-config sdl2 --cflags)
SDL2_LIBS    := $(shell x86_64-w64-mingw32-pkg-config sdl2 --libs --static)
SHARED_FLAGS := -g -O0 -Wall -Werror -pedantic-errors $(SDL2_CFLAGS)
CFLAGS       := $(SHARED_FLAGS) -std=c11
CXXFLAGS     := $(SHARED_FLAGS) -std=c++11
CPPFLAGS     := -DDEBUG -Iglsys/include -Iexternal/glm-0.9.8.4 -Iexternal/khronos/include -Icommon
GLSYS_LIB    := glsys/src/glsys.a
LDLIBS       := -lopengl32 $(SDL2_LIBS) -static
LINK.o       := $(LINK.cc)

EXECUTABLES := glsys/src/glsys.a
EXECUTABLES := $(EXECUTABLES) test/HelloWorld
EXECUTABLES := $(EXECUTABLES) 010-using_function_loader/TestFunctionLoader
EXECUTABLES := $(EXECUTABLES) 013-using_glm/TestGlm
EXECUTABLES := $(EXECUTABLES) 015-determining_version/DetermineVersion
EXECUTABLES := $(EXECUTABLES) 017-compiling_a_shader/CompilingAShader
EXECUTABLES := $(EXECUTABLES) 021-linking_a_shader/LinkingAShader
EXECUTABLES := $(EXECUTABLES) 025-sending_data/SendingData
EXECUTABLES := $(EXECUTABLES) 034-active_attributes/ListActiveAttributes
EXECUTABLES := $(EXECUTABLES) 037-using_uniforms/UsingUniforms
EXECUTABLES := $(EXECUTABLES) 041-active_uniforms/ListActiveUniforms
EXECUTABLES := $(EXECUTABLES) 043-uniform_blocks/UniformBlocks

.PHONY: all clean

all: $(EXECUTABLES)

clean:
	find . -name "*.o" -type f -delete
	rm -f $(EXECUTABLES)

glsys/src/glsys.a: glsys/src/glsys.o glsys/src/glsys.generated.o
	$(AR) rcs $@ $^

test/HelloWorld: test/HelloWorld.o $(GLSYS_LIB)

010-using_function_loader/TestFunctionLoader: 010-using_function_loader/TestFunctionLoader.o $(GLSYS_LIB)

013-using_glm/TestGlm: 013-using_glm/TestGlm.o $(GLSYS_LIB)

015-determining_version/DetermineVersion: 015-determining_version/DetermineVersion.o $(GLSYS_LIB)

017-compiling_a_shader/CompilingAShader: 017-compiling_a_shader/CompilingAShader.o common/FileUtil.o $(GLSYS_LIB)

021-linking_a_shader/LinkingAShader: 021-linking_a_shader/LinkingAShader.o common/FileUtil.o $(GLSYS_LIB)

025-sending_data/SendingData: 025-sending_data/SendingData.o common/FileUtil.o $(GLSYS_LIB)

034-active_attributes/ListActiveAttributes: 034-active_attributes/ListActiveAttributes.o common/FileUtil.o $(GLSYS_LIB)

037-using_uniforms/UsingUniforms: 037-using_uniforms/UsingUniforms.o common/FileUtil.o $(GLSYS_LIB)

041-active_uniforms/ListActiveUniforms: 041-active_uniforms/ListActiveUniforms.o common/FileUtil.o $(GLSYS_LIB)

043-uniform_blocks/UniformBlocks: 043-uniform_blocks/UniformBlocks.o common/FileUtil.o $(GLSYS_LIB)

