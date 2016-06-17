#include <GLES2/gl2.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_timer.h"
#include "matrix.h"

typedef enum EVertexAttrIndex {
    VERTEX_ATTR_INDEX_POSITION,
    VERTEX_ATTR_INDEX_COLOR
} EVertexAttrIndex;

#define VERTEX_NUM_COMPONENTS_POSITION 3
#define VERTEX_NUM_COMPONENTS_COLOR    3
#define VERTEX_STRIDE_BYTES            (sizeof(GLfloat) * (VERTEX_NUM_COMPONENTS_POSITION + VERTEX_NUM_COMPONENTS_COLOR))

static GLfloat const skTriangleInterleavedMesh[] = {
    0.0f, 0.5f, 0.0f,
    1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.0f, 1.0f};

static GLushort const skTriangleIndices[] = {0, 1, 2};

static void      draw(void);
static void      simulationLoop(SDL_Window *window);
static void      setupBuffers(void);
static GLchar   *loadTextFileIntoString(const char *filename);
static GLuint    loadShader(const GLenum shader_type, const char *filename);
static GLuint    loadProgram(const char *vertex_shader_filename, const char *fragment_shader_filename);

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

static GLuint
loadShader(const GLenum shaderType, const char *filename)
{
    GLuint const shaderHandle = glCreateShader(shaderType);
    assert(shaderHandle != 0);

    GLchar const *fileBuffer = loadTextFileIntoString(filename);
    glShaderSource(shaderHandle, 1, &fileBuffer, NULL);
    free((void*)fileBuffer);

    glCompileShader(shaderHandle);
    GLint compileStatus = 0;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        printf("Error compiling %s\n", filename);
    }
    GLint infoLogLength = 0;
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 1) {
        char * const infoLogBuffer = (char *)malloc((GLuint)infoLogLength);
        glGetShaderInfoLog(shaderHandle, infoLogLength, NULL, infoLogBuffer);
        printf("Info log for shader %s\n", filename);
        puts(infoLogBuffer);
        free(infoLogBuffer);
    }
    assert(compileStatus == GL_TRUE);

    return shaderHandle;
}
 
static GLuint
loadProgram(const char *vertexShaderFilename, const char *fragmentShaderFilename)
{
    GLuint const vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFilename);
    assert(vertexShader != 0);
    GLuint const fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
    assert(fragmentShader != 0);
    GLuint const programHandle = glCreateProgram();
    assert(programHandle != 0);

    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_POSITION, "aPosition");
    glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_COLOR, "aColor");
    glLinkProgram(programHandle);
    
    GLint linkStatus = 0;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
    GLint infoLogLength = 0;
    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 1) {
        char *infoLogBuffer = (char *)malloc((GLuint)infoLogLength);
        glGetProgramInfoLog(programHandle, infoLogLength, NULL, infoLogBuffer);
        printf("\n\nInfo log for program %u (vert: %s frag %s)\n", programHandle, vertexShaderFilename, fragmentShaderFilename);
        printf("%s\n", infoLogBuffer);
        free(infoLogBuffer);
    }

    assert(linkStatus == GL_TRUE);
    return programHandle;
}

static GLint sRotationIndex;
static Uint32 sNextReset = 0;

static void
draw(void)
{
    TmMat4 rotationMatrix;
    static TmVec3 const yAxis = {0.0f, 1.0f, 0.0};

    Uint32 const currentTicks = SDL_GetTicks();
	static Uint32 const skRotationTicks = 2000;
    if (currentTicks > sNextReset) {
        sNextReset = currentTicks + skRotationTicks;
    }
    float const rotationPercentage = (sNextReset - currentTicks) / (float) skRotationTicks;
    
    tmMat4Rotation(&rotationMatrix, &yAxis, (2 * TM_SCALAR_PI) * rotationPercentage);
    glUniformMatrix4fv(sRotationIndex, 1, GL_FALSE, (GLfloat*)&rotationMatrix);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, sizeof(skTriangleIndices) / sizeof(skTriangleIndices[0]), GL_UNSIGNED_SHORT, (const void *)0);
}

static void
simulationLoop(SDL_Window *window)
{
    SDL_Event event;
    bool isDone = false;

    while (!isDone) {
        while(SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT)
                isDone = true;
            else if (event.type == SDL_KEYDOWN)
                isDone = true;
        }
        draw();
        SDL_GL_SwapWindow(window);
    }
}

static void
setupBuffers(void)
{

    GLuint bufferObjects[2];
    glGenBuffers(2, bufferObjects);

    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skTriangleInterleavedMesh), skTriangleInterleavedMesh, GL_STATIC_DRAW);
    
    intptr_t offsetIntoBuffer = 0;
    glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION,
                          VERTEX_NUM_COMPONENTS_POSITION,
                          GL_FLOAT,
                          GL_FALSE,
                          VERTEX_STRIDE_BYTES,
                          (const void *)offsetIntoBuffer);
    offsetIntoBuffer += sizeof(GLfloat) * VERTEX_NUM_COMPONENTS_POSITION;
    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_COLOR,
                          VERTEX_NUM_COMPONENTS_COLOR,
                          GL_FLOAT,
                          GL_FALSE,
                          VERTEX_STRIDE_BYTES,
                          (const void *)offsetIntoBuffer);
    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skTriangleIndices), skTriangleIndices, GL_STATIC_DRAW);
}

int
main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
 
    int ret = SDL_Init(SDL_INIT_VIDEO);
    assert(ret == 0);
  
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window * const mainWindow = SDL_CreateWindow("Yeah buddy",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        512,
        512,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    assert(mainWindow != NULL);
 
    SDL_GLContext const mainContext = SDL_GL_CreateContext(mainWindow);

    GLuint const shaderProgram = loadProgram("shaders/rotate_triangle.vert", "shaders/rotate_triangle.frag");
    glUseProgram(shaderProgram);
    sRotationIndex = glGetUniformLocation(shaderProgram, "uRotation");

    setupBuffers();
     
    SDL_GL_SetSwapInterval(1); /* Sync with the refresh rate */

    simulationLoop(mainWindow);
 
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
 
    return 0;
}
