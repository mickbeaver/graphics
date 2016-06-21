#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

#include "SDL.h"
#include "framework.h"
#include "glsys.h"

static void      simulationLoop(SDL_Window *window);
static bool      handleEvent(SDL_Event *event);
static GLchar   *loadTextFileIntoString(const char *filename);

static GLchar *
loadTextFileIntoString(const char *filename)
{
    int ret;

    FILE * const inputObj = fopen(filename, "rb");
    assert(inputObj != NULL);
    ret = fseek(inputObj, 0, SEEK_END);
    assert(ret == 0);
    long const fileLength = ftell(inputObj);
    assert(fileLength > 0);
    ret = fseek(inputObj, 0, SEEK_SET);
    assert(ret == 0);
    char * const buffer = (GLchar *)malloc((unsigned long)fileLength + 1 /*NUL*/);
    
    size_t totalBytesRead = 0;
    do {
        size_t const currentBytesRead = fread(buffer, 1, (unsigned long)fileLength, inputObj);
        if (currentBytesRead == 0 && ferror(inputObj) != 0) {
            printf("Error reading from %s. errno is %d\n", filename, errno);
            assert(0);
        }
        totalBytesRead += currentBytesRead;
    } while (totalBytesRead < (size_t)fileLength);

    fclose(inputObj);
    buffer[fileLength] = '\0';

    return buffer;
}

GLuint
frameworkLoadShader(GLenum shaderType, const char *filename)
{
    GLuint shader;
    GLint status;
    const GLchar *shaderSource;

    shader = glCreateShader(shaderType);
    shaderSource = loadTextFileIntoString(filename);
    glShaderSource(shader, 1, &shaderSource, NULL);

    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        GLchar *strInfoLog;
        const char *strShaderType;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        strInfoLog = (GLchar *)malloc(infoLogLength + 1);
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
        switch (shaderType) {
        case GL_VERTEX_SHADER:
            strShaderType = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            strShaderType = "fragment";
            break;
        default:
            strShaderType = "unknown";
            break;
        }
        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        free(strInfoLog);
    }
    free((void *)shaderSource);

    return shader;
}

GLuint
frameworkCreateProgram(GLuint vertexShader,
                       GLuint fragmentShader,
                       const FrameworkShaderAttribLocation *attribLocations,
                       size_t attribLocationCount)
{
    GLuint program;
    GLint status;

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    for (size_t i = 0; i < attribLocationCount; ++i) {
        glBindAttribLocation(program, attribLocations[i].location, attribLocations[i].name);
    }
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        GLchar *strInfoLog;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        strInfoLog = (char *)malloc(infoLogLength + 1);
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        free(strInfoLog);
    }
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}

static bool
handleEvent(SDL_Event *event)
{
    bool isDone = false;
    
    switch(event->type) {
    case SDL_QUIT:
    case SDL_KEYDOWN:
        isDone = true;
        break;
    case SDL_WINDOWEVENT:
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            gltutReshape(event->window.data1, event->window.data2);
        }
        break;
    default:
        break;
    }

    return isDone;
}

static void
simulationLoop(SDL_Window *window)
{
	SDL_Event event;
	bool isDone = false;

	while (!isDone) {
		while(SDL_PollEvent(&event) == 1 && !isDone) {
            isDone = handleEvent(&event);
		}
        gltutDisplay();
		SDL_GL_SwapWindow(window);
	}
}

int
main(int argc, char** argv)
{
    GltutDefaultSettings settings = GLTUT_DEFAULT_SETTINGS_INITIALIZER;
    SDL_GLContext mainContext;
    SDL_Window *mainWindow;
    int retval;
    const char *errorStr;

	(void) argc;
	(void) argv;

    gltutDefaultSettingsInit(&settings);

    retval = SDL_Init(SDL_INIT_VIDEO);
    assert(retval == 0);
    
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    mainWindow = SDL_CreateWindow(settings.windowTitle,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  settings.windowWidth,
                                  settings.windowHeight,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    assert(mainWindow != NULL);

    mainContext = SDL_GL_CreateContext(mainWindow);
    errorStr = SDL_GetError();
    if (*errorStr != '\0') {
    fprintf(stderr, "SDL_GetError() returned: %s\n", errorStr);
        //assert(0);
    }

    gltutPostRenderSystemInit();

    simulationLoop(mainWindow);

    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
