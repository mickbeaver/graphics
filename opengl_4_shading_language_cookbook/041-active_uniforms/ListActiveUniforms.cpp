#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <SDL2/SDL.h>

#include "glsys.h"
#include "FileUtil.h"

#define WINDOW_SIZE 640
#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

struct ShaderProgramSource {
    GLenum shaderType;
    const char* sourceFilename;
};

void printShaderInfoLog(GLuint shaderHandle, const char* name)
{
    GLint logLength;
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        char* logBuffer = new char[logLength];
        GLsizei bytesCopied;
        glGetShaderInfoLog(shaderHandle, logLength, &bytesCopied, logBuffer);
        (void)fprintf(stderr, "Shader '%s' info log:\n%s\n", name, logBuffer);
        delete [] logBuffer;
    }
}

GLuint compileShader(GLenum shaderType, const char* filename)
{
    GLuint shaderHandle = glCreateShader(shaderType);
    if (shaderHandle != 0) {
        const GLchar* shaderCode = FileUtil::loadFileAsString(filename);
        glShaderSource(shaderHandle, 1, &shaderCode, NULL);
        delete [] shaderCode;
        glCompileShader(shaderHandle);

        GLint result;
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
        if (result == GL_TRUE) {
            (void)printf("Shader '%s' compilation was successful!\n", filename);
            // Print warnings, if available
            printShaderInfoLog(shaderHandle, filename);
        } else {
            (void)fprintf(stderr, "Shader compilation failed for '%s'!\n", filename);
            printShaderInfoLog(shaderHandle, filename);
            glDeleteShader(shaderHandle);
            shaderHandle = 0;
        }
    } else {
        (void)fprintf(stderr, "Error creating shader for '%s'.\n", filename);
    }
    return shaderHandle;
}

void printProgramInfoLog(GLuint programHandle)
{
    GLint logLength;
    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        char* logBuffer = new char[logLength];
        GLsizei bytesCopied;
        glGetProgramInfoLog(programHandle, logLength, &bytesCopied, logBuffer);
        (void)fprintf(stderr, "Program info log:\n%s\n", logBuffer);
        delete [] logBuffer;
    }
}

GLuint linkShaderProgram(const ShaderProgramSource* sources, size_t numSources)
{
    assert(numSources != 0);
    GLuint programHandle = glCreateProgram();
    if (programHandle != 0) {
        for (size_t i = 0; i < numSources; ++i) {
            GLuint shaderHandle = compileShader(sources[i].shaderType, sources[i].sourceFilename);
            if (shaderHandle != 0) {
                glAttachShader(programHandle, shaderHandle);
                glDeleteShader(shaderHandle);
            }
        }
        glLinkProgram(programHandle);

        GLint linkStatus;
        glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_TRUE) {
            (void)printf("Shader link was successful!\n");
            printProgramInfoLog(programHandle);
        } else {
            (void)fprintf(stderr, "Shader link failed!\n");
            printProgramInfoLog(programHandle);
            glDeleteProgram(programHandle);
            programHandle = 0;
        }
    } else {
        (void)fprintf(stderr, "Error creating shader program handle.\n");
        exit(EXIT_FAILURE);
    }
    
    return programHandle;
}

void printActiveUniforms(GLuint programHandle)
{
    GLint numUniforms = 0;
    glGetProgramInterfaceiv(programHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
    
    if (numUniforms > 0) {
        static GLenum const properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};
        struct PropertyResults {
            GLint nameLength;
            GLint type;
            GLint location;
            GLint blockIndex;
        } results;
        static_assert(ARRAY_COUNT(properties) == sizeof(results)/sizeof(results.nameLength), "PropertyResults is not the right size.");
        (void)printf("Active uniforms\n");
        for (int i = 0; i < numUniforms; i++) {
            glGetProgramResourceiv(programHandle,
                                   GL_UNIFORM,
                                   i,
                                   ARRAY_COUNT(properties),
                                   properties,
                                   ARRAY_COUNT(properties),
                                   NULL,
                                   reinterpret_cast<GLint*>(&results));

            if (results.blockIndex == -1) {
                // Single uniform
                assert(results.nameLength > 0);
                char* nameBuffer = new char[results.nameLength];
                glGetProgramResourceName(programHandle, GL_UNIFORM, i, results.nameLength, NULL, nameBuffer);
                printf("location=%d name=%s (%s)\n", results.location, nameBuffer, glsysGetTypeString(results.type));
                delete [] nameBuffer;
            } else {
                // Uniform block member
                GLenum blockNameProperty = GL_NAME_LENGTH;
                GLint blockNameLength;
                glGetProgramResourceiv(programHandle, GL_UNIFORM_BLOCK, results.blockIndex, 1, &blockNameProperty, 1, NULL, &blockNameLength);
                assert(blockNameLength > 0);
                char* blockNameBuffer = new char[blockNameLength];
                glGetProgramResourceName(programHandle, GL_UNIFORM_BLOCK, results.blockIndex, blockNameLength, NULL, blockNameBuffer);

                char* nameBuffer = new char[results.nameLength];
                glGetProgramResourceName(programHandle, GL_UNIFORM, i, results.nameLength, NULL, nameBuffer);
                printf("block=%s %s (%s)\n", blockNameBuffer, nameBuffer, glsysGetTypeString(results.type));
                delete [] nameBuffer;
                delete [] blockNameBuffer;
            }
        }
    }
}

int
main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    int retval = SDL_Init(SDL_INIT_VIDEO);
    if (retval != 0) {
        (void)fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    Uint32 glContextFlags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#ifdef DEBUG
    glContextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glContextFlags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_ClearError();
    SDL_Window* mainWindow = SDL_CreateWindow("Listing Active Uniforms",
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              WINDOW_SIZE,
                                              WINDOW_SIZE,
                                              SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (mainWindow == NULL) {
        (void)fprintf(stderr, "SDL_CreateWindow() failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
    if (mainContext == NULL) {
        (void)fprintf(stderr, "SDL_GL_CreateContext() failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    retval = glsysLoadFunctions((glsysFunctionLoader)SDL_GL_GetProcAddress);
    if (retval != 0) {
        (void)fprintf(stderr, "Error loading GL functions.\n");
        exit(EXIT_FAILURE);
    }

    ShaderProgramSource programSources[] = {{GL_VERTEX_SHADER, "basic.vert"},
                                            {GL_FRAGMENT_SHADER, "basic.frag"}};
    GLuint programHandle = linkShaderProgram(programSources, ARRAY_COUNT(programSources));
    if (programHandle != 0) {
        printActiveUniforms(programHandle);
    }
    
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
