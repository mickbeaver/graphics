#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "FileUtil.h"

#define WINDOW_SIZE 640
#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

struct ShaderProgramSource {
    GLenum shaderType;
    const char* sourceFilename;
};

void printShaderInfoLog(GLuint shaderHandle, const char* name) {
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

GLuint compileShader(GLenum shaderType, const char* filename) {
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

void printProgramInfoLog(GLuint programHandle) {
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

GLuint linkShaderProgram(const ShaderProgramSource* sources, size_t numSources) {
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

class Simulation {
public:
    Simulation(SDL_Window* window);
    virtual ~Simulation();

    void runLoop();

private:
    bool handleEvent(SDL_Event* event);
    void initializeBuffers();
    void render();
    
    SDL_Window* m_window;
    GLuint m_programHandle;
    GLuint m_vboHandles[2];
    GLuint m_vaoHandle;
};

bool Simulation::handleEvent(SDL_Event *event)
{
    bool isDone = false;
    
    switch(event->type) {
    case SDL_QUIT:
        isDone = true;
        break;
    case SDL_KEYDOWN:
        if (event->key.keysym.sym == SDLK_ESCAPE) {
            isDone = true;
        }
        break;
    case SDL_WINDOWEVENT:
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            // TODO Handle reshape
        }
        break;
    default:
        break;
    }

    return isDone;
}
void Simulation::render()
{
    glUseProgram(m_programHandle);
    glBindVertexArray(m_vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Simulation::runLoop()
{
    SDL_Event event;
    bool isDone = false;

    while (!isDone) {
        while(SDL_PollEvent(&event) == 1 && !isDone) {
            isDone = handleEvent(&event);
        }
        render();
        SDL_GL_SwapWindow(m_window);
    }
}

void Simulation::initializeBuffers()
{
    const float positionData[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f,  0.0f,
        0.0f, 0.8f, 0.0f
    };
    const float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glGenBuffers(ARRAY_COUNT(m_vboHandles), m_vboHandles);
    GLuint& positionBufferHandle = m_vboHandles[0];
    GLuint& colorBufferHandle = m_vboHandles[1];

    // Position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

    // Color buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

    // Vertex array object
    glGenVertexArrays(1, &m_vaoHandle);
    glBindVertexArray(m_vaoHandle);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Simulation::Simulation(SDL_Window* window)
  : m_window(window)
  , m_programHandle(0)
{
    ShaderProgramSource programSources[] = {{GL_VERTEX_SHADER, "basic.vert"},
                                            {GL_FRAGMENT_SHADER, "basic.frag"}};
    m_programHandle = linkShaderProgram(programSources, ARRAY_COUNT(programSources));

    initializeBuffers();
    fflush(NULL);
}

Simulation::~Simulation()
{
    glDeleteVertexArrays(1, &m_vaoHandle);
    glDeleteBuffers(ARRAY_COUNT(m_vboHandles), m_vboHandles);
    glDeleteProgram(m_programHandle);
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    printf("Entering main\n");

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
    SDL_Window* mainWindow = SDL_CreateWindow("Sending Data to a Shader",
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              WINDOW_SIZE,
                                              WINDOW_SIZE,
                                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (mainWindow == NULL) {
        (void)fprintf(stderr, "SDL_CreateWindow() failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
    if (mainContext == NULL) {
        (void)fprintf(stderr, "SDL_GL_CreateContext() failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    fflush(NULL);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        (void)fprintf(stderr, "Error Initializing GLEW: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    printf("Starting simulation\n");


    fflush(NULL);

    Simulation* simulation = new Simulation(mainWindow);
    simulation->runLoop();
    delete simulation;
    printf("Ending simulation\n");
    fflush(NULL);
    
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
