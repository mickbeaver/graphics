#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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
    GLuint m_vertexBufferHandle;
    GLuint m_indexBufferHandle;
    GLuint m_vaoHandle;
    GLsizei m_numVertices;
    GLuint m_uniformBufferHandle;
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
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_programHandle);
    glBindVertexArray(m_vaoHandle);
    glDrawElements(GL_TRIANGLES, m_numVertices, GL_UNSIGNED_SHORT, (const GLvoid*)0);
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
    static float const vertexData[] = {
        -1.0f, 1.0f, 0.0f,  // 0: pos
        0.0f, 1.0f,         // 0: texcoord
        -1.0f, -1.0f, 0.0f, // 1: pos
        0.0f, 0.0f,         // 1: texcoord
        1.0f, 1.0f, 0.0f,   // 2: pos
        1.0f, 1.0f,         // 2: texcoord
        1.0f, -1.0f, 0.0f,  // 3: pos
        1.0f, 0.0f,         // 3: texcoord
    };
    static size_t const vertexStride = 5 * sizeof(GLfloat);
    static GLuint const positionAttribIndex = 0;
    static GLuint const positionOffset = 0;
    static GLuint const texCoordAttribIndex = 1;
    static GLuint const texCoordOffset = 3 * sizeof(GLfloat);

    static uint16_t const indices[] = {
        0, 1, 2,
        2, 1, 3
    };

    glCreateBuffers(1, &m_vertexBufferHandle);
    glCreateBuffers(1, &m_indexBufferHandle);
    glCreateBuffers(1, &m_uniformBufferHandle);
    glCreateVertexArrays(1, &m_vaoHandle);

    GLuint bindingIndex = 0;
    glNamedBufferData(m_vertexBufferHandle, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(m_vaoHandle, bindingIndex, m_vertexBufferHandle, 0, vertexStride);

    glEnableVertexArrayAttrib(m_vaoHandle, positionAttribIndex);
    glVertexArrayAttribBinding(m_vaoHandle, positionAttribIndex, bindingIndex);
    glVertexArrayAttribFormat(m_vaoHandle, positionAttribIndex, 3, GL_FLOAT, GL_FALSE, positionOffset);

    glEnableVertexArrayAttrib(m_vaoHandle, texCoordAttribIndex);
    glVertexArrayAttribBinding(m_vaoHandle, texCoordAttribIndex, bindingIndex);
    glVertexArrayAttribFormat(m_vaoHandle, texCoordAttribIndex, 2, GL_FLOAT, GL_FALSE, texCoordOffset);

    glNamedBufferData(m_indexBufferHandle, sizeof(indices), indices, GL_STATIC_DRAW);
    m_numVertices = ARRAY_COUNT(indices);
    glVertexArrayElementBuffer(m_vaoHandle, m_indexBufferHandle);

    GLuint blockIndex = glGetUniformBlockIndex(m_programHandle, "BlobSettings");
    (void)printf("BlobSettings is at block index=%u\n", blockIndex);
    GLint blockSize;
    glGetActiveUniformBlockiv(m_programHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

#if 1
    struct UniformBlockStd140 {
        GLfloat m_innerColor[4];
        GLfloat m_outerColor[4];
        GLfloat m_radiusInner;
        GLfloat m_radiusOuter;
    };
    UniformBlockStd140 uniformBlock = {{1.0f, 1.0f, 0.75f, 1.0f},
                                       {0.0f, 0.0f, 0.0f, 0.0f},
                                       0.25f,
                                       0.45f};
    glNamedBufferData(m_uniformBufferHandle, sizeof(uniformBlock), &uniformBlock, GL_DYNAMIC_DRAW);
#else
    static GLfloat const innerColor[] = {1.0f, 1.0f, 0.75f, 1.0f};
    static GLfloat const outerColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    static GLfloat const innerRadius = 0.25f;
    static GLfloat const outerRadius = 0.45f;

    const GLchar* uniformNames[] = {"InnerColor", "OuterColor", "RadiusInner", "RadiusOuter"};
    GLuint uniformIndices[ARRAY_COUNT(uniformNames)];
    glGetUniformIndices(m_programHandle, ARRAY_COUNT(uniformNames), uniformNames, uniformIndices);
    GLint uniformOffsets[ARRAY_COUNT(uniformIndices)];
    glGetActiveUniformsiv(m_programHandle, ARRAY_COUNT(uniformIndices), uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);
    
    GLubyte* blockBuffer = new GLubyte[blockSize];
    (void)printf("InnerColor is at offset=%d\n", uniformOffsets[0]);
    (void)memcpy(blockBuffer + uniformOffsets[0], innerColor, sizeof(innerColor));
    (void)printf("OuterColor is at offset=%d\n", uniformOffsets[1]);
    (void)memcpy(blockBuffer + uniformOffsets[1], outerColor, sizeof(outerColor));
    (void)printf("RadiusInner is at offset=%d\n", uniformOffsets[2]);
    (void)memcpy(blockBuffer + uniformOffsets[2], &innerRadius, sizeof(innerRadius));
    (void)printf("RadiusOuter is at offset=%d\n", uniformOffsets[3]);
    (void)memcpy(blockBuffer + uniformOffsets[3], &outerRadius, sizeof(outerRadius));
    bool testBool = true;
    (void)memcpy(blockBuffer + uniformOffsets[4], &testBool, sizeof(testBool));
    glNamedBufferData(m_uniformBufferHandle, blockSize, blockBuffer, GL_DYNAMIC_DRAW);
    delete [] blockBuffer;
#endif
    GLint blockBindingIndex;
    glGetActiveUniformBlockiv(m_programHandle, blockIndex, GL_UNIFORM_BLOCK_BINDING, &blockBindingIndex);
    (void)printf("The block binding index=%d\n", blockBindingIndex);
    glBindBufferBase(GL_UNIFORM_BUFFER, blockBindingIndex, m_uniformBufferHandle);
}

Simulation::Simulation(SDL_Window* window)
  : m_window(window)
  , m_programHandle(0)
  , m_vertexBufferHandle(0)
  , m_indexBufferHandle(0)
  , m_vaoHandle(0)
  , m_numVertices(0)
  , m_uniformBufferHandle(0)
{
    ShaderProgramSource programSources[] = {{GL_VERTEX_SHADER, "circle.vert"},
                                            {GL_FRAGMENT_SHADER, "circle.frag"}};
    m_programHandle = linkShaderProgram(programSources, ARRAY_COUNT(programSources));

    initializeBuffers();
}

Simulation::~Simulation()
{
    glDeleteVertexArrays(1, &m_vaoHandle);
    glDeleteBuffers(1, &m_vertexBufferHandle);
    glDeleteBuffers(1, &m_indexBufferHandle);
    glDeleteBuffers(1, &m_uniformBufferHandle);
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
    SDL_Window* mainWindow = SDL_CreateWindow("Using Uniform Blocks",
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
