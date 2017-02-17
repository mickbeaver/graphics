#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "FileUtil.h"

#define WINDOW_SIZE 640

void printShaderInfoLog(GLuint shaderHandle) {
    GLint logLength;
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        char* logBuffer = new char[logLength];
        GLsizei bytesCopied;
        glGetShaderInfoLog(shaderHandle, logLength, &bytesCopied, logBuffer);
        (void)fprintf(stderr, "Shader info log:\n%s\n", logBuffer);
        delete [] logBuffer;
        exit(EXIT_FAILURE);
    }
}

int
main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int retval = SDL_Init(SDL_INIT_VIDEO);
    if (retval != 0) {
        (void)fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_ClearError();
    SDL_Window* mainWindow = SDL_CreateWindow("Compiling a Shader",
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

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        (void)fprintf(stderr, "Error Initializing GLEW: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader == 0) {
        (void)fprintf(stderr, "Error creating vertex shader.\n");
        exit(EXIT_FAILURE);
    }
    const GLchar* shaderCode = FileUtil::loadFileAsString("basic.vert");
    glShaderSource(vertShader, 1, &shaderCode, NULL);
    delete [] shaderCode;
    glCompileShader(vertShader);

    GLint result;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        (void)fprintf(stderr, "Vertex shader compilation failed!\n");
        printShaderInfoLog(vertShader);
    } else {
        (void)printf("Shader compiled successfully!\n");
        // Print warnings, if available
        printShaderInfoLog(vertShader);
    }
   
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
