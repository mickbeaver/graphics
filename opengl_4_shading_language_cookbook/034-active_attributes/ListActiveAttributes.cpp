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

void printActiveAttributes(GLuint programHandle)
{
#if 0
    GLint maxLength;
    GLint numAttribs;

    glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES, &numAttribs);
    glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    GLchar* nameBuffer = new GLchar[maxLength];

    (void)printf(" Location | Name\n");
    for (int i = 0; i < numAttribs; i++) {
        GLenum type;
        GLint bytesWritten;
        GLint size;
        GLint location;

        glGetActiveAttrib(programHandle, i, maxLength, &bytesWritten, &size, &type, nameBuffer);
        location = glGetAttribLocation(programHandle, nameBuffer);
        (void)printf(" %-8d | %s\n", location, nameBuffer);
    }
    
    delete [] nameBuffer;
#else
    GLint numAttribs;
    glGetProgramInterfaceiv(programHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

    //GLint maxLength;
    //glGetProgramInterfaceiv(programHandle, GL_PROGRAM_INPUT, GL_MAX_NAME_LENGTH, &maxLength);

    GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};
    (void)printf("Active input resources:\n");
    for (int i = 0; i < numAttribs; i++) {
        GLint propertyValues[ARRAY_COUNT(properties)];
        glGetProgramResourceiv(programHandle,
                               GL_PROGRAM_INPUT,
                               i,
                               ARRAY_COUNT(properties),
                               properties,
                               ARRAY_COUNT(propertyValues),
                               NULL,
                               propertyValues);
        char* nameBuffer = new char[propertyValues[0]];
        glGetProgramResourceName(programHandle, GL_PROGRAM_INPUT, i, propertyValues[0], NULL, nameBuffer);
        (void)printf("%-5d %s (%s)\n", propertyValues[2], nameBuffer, glsysGetTypeString(propertyValues[1]));
        delete [] nameBuffer;
    }
#endif
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
    SDL_Window* mainWindow = SDL_CreateWindow("Listing Vertex Attributes",
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
        printActiveAttributes(programHandle);
    }
    
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
