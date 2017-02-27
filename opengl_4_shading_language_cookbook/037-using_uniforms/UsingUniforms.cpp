
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

#include "glsys.h"
#include "FileUtil.h"

#define WINDOW_SIZE 640
#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))
#define DSA_ENABLED 1

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
    static const uint32_t ROTATION_TIME_MS = 2000;
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
    Uint32 const currentTicks = SDL_GetTicks() % ROTATION_TIME_MS;
    float rotationPercentage = currentTicks / static_cast<float>(ROTATION_TIME_MS);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_programHandle);
    float const rotationAngle = glm::two_pi<float>() * rotationPercentage;
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f),
                                           rotationAngle,
                                           glm::vec3(0.0f, 0.0f, 1.0f));
    GLint const location = glGetUniformLocation(m_programHandle, "RotationMatrix");
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
    } else {
        assert(0);
    }
    
    glBindVertexArray(m_vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);

    SDL_GL_SwapWindow(m_window);
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
    }
}

void Simulation::initializeBuffers()
{
    const float positionData[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f,  0.0f,
        0.0f, 0.8f, 0.0f
    };
    const size_t positionStride = 3 * sizeof(GLfloat);
    const GLuint positionAttribIndex = 0;

    const float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    const size_t colorStride = 3 * sizeof(GLfloat);
    const GLuint colorAttribIndex = 1;

#if DSA_ENABLED
    glCreateBuffers(ARRAY_COUNT(m_vboHandles), m_vboHandles);
    GLuint& positionBufferHandle = m_vboHandles[0];
    GLuint& colorBufferHandle = m_vboHandles[1];
    glCreateVertexArrays(1, &m_vaoHandle);

    GLuint bindingIndex = 0;
    glNamedBufferData(positionBufferHandle, sizeof(positionData), positionData, GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(m_vaoHandle, positionAttribIndex);
    glVertexArrayVertexBuffer(m_vaoHandle, bindingIndex, positionBufferHandle, 0, positionStride);
    glVertexArrayAttribBinding(m_vaoHandle, positionAttribIndex, bindingIndex);
    glVertexArrayAttribFormat(m_vaoHandle, positionAttribIndex, 3, GL_FLOAT, GL_FALSE, 0);
   
    bindingIndex++;
    glNamedBufferData(colorBufferHandle, sizeof(colorData), colorData, GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(m_vaoHandle, colorAttribIndex);
    glVertexArrayVertexBuffer(m_vaoHandle, bindingIndex, colorBufferHandle, 0, colorStride);
    glVertexArrayAttribBinding(m_vaoHandle, colorAttribIndex, bindingIndex);
    glVertexArrayAttribFormat(m_vaoHandle, colorAttribIndex, 3, GL_FLOAT, GL_FALSE, 0);
#else
    glGenBuffers(ARRAY_COUNT(m_vboHandles), m_vboHandles);
    GLuint& positionBufferHandle = m_vboHandles[0];
    GLuint& colorBufferHandle = m_vboHandles[1];
    glGenVertexArrays(1, &m_vaoHandle);

    glBindVertexArray(m_vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(positionAttribIndex);
    glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, positionStride, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorAttribIndex);
    glVertexAttribPointer(colorAttribIndex, 3, GL_FLOAT, GL_FALSE, colorStride, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
#endif
}

Simulation::Simulation(SDL_Window* window)
  : m_window(window)
  , m_programHandle(0)
{

    ShaderProgramSource programSources[] = {{GL_VERTEX_SHADER, "basic.vert"},
                                            {GL_FRAGMENT_SHADER, "basic.frag"}};
    m_programHandle = linkShaderProgram(programSources, ARRAY_COUNT(programSources));

    initializeBuffers();
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
    Uint32 glContextFlags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#if DEBUG
    glContextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glContextFlags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_ClearError();
    SDL_Window* mainWindow = SDL_CreateWindow("Using Uniforms",
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

    retval = glsysLoadFunctions((glsysFunctionLoader)SDL_GL_GetProcAddress);
    if (retval != 0) {
        (void)fprintf(stderr, "Error loading GL functions.\n");
        exit(EXIT_FAILURE);
    }

#if DEBUG
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glsysDebugCallback, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif
    
    printf("Starting simulation\n");
    Simulation* simulation = new Simulation(mainWindow);
    simulation->runLoop();
    delete simulation;
    printf("Ending simulation\n");
    
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
