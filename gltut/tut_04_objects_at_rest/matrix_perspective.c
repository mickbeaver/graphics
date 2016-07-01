#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL_timer.h"
#include "framework.h"
#include "glsys.h"

#define ARRAY_COUNT(array) (sizeof(array)/sizeof(array[0]))

typedef enum EVertexAttrIndex {
    VERTEX_ATTR_INDEX_POSITION,
    VERTEX_ATTR_INDEX_COLOR
} EVertexAttrIndex;

static FrameworkShaderAttribLocation const skShaderAttribLocations[] = {
    {.name="a_position", .location=VERTEX_ATTR_INDEX_POSITION},
    {.name="a_color", .location=VERTEX_ATTR_INDEX_COLOR}
};

static float const skVertexData[] = {
     0.25f,  0.25f, -1.25f, 1.0f,
     0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f,  0.25f, -1.25f, 1.0f,

     0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f,  0.25f, -1.25f, 1.0f,

     0.25f,  0.25f, -2.75f, 1.0f,
    -0.25f,  0.25f, -2.75f, 1.0f,
     0.25f, -0.25f, -2.75f, 1.0f,

     0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f,  0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,

    -0.25f,  0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,

    -0.25f,  0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f,  0.25f, -2.75f, 1.0f,

     0.25f,  0.25f, -1.25f, 1.0f,
     0.25f, -0.25f, -2.75f, 1.0f,
     0.25f, -0.25f, -1.25f, 1.0f,

     0.25f,  0.25f, -1.25f, 1.0f,
     0.25f,  0.25f, -2.75f, 1.0f,
     0.25f, -0.25f, -2.75f, 1.0f,

     0.25f,  0.25f, -2.75f, 1.0f,
     0.25f,  0.25f, -1.25f, 1.0f,
    -0.25f,  0.25f, -1.25f, 1.0f,

     0.25f,  0.25f, -2.75f, 1.0f,
    -0.25f,  0.25f, -1.25f, 1.0f,
    -0.25f,  0.25f, -2.75f, 1.0f,

     0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,
     0.25f, -0.25f, -1.25f, 1.0f,

     0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,

    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,

    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,

    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
};

static GLuint sTheProgram;

static GLint  sOffsetUniform;
static GLint  sPerspectiveMatrixUniform;

static GLuint sPositionBufferObject;

static void     initializeProgram();
static void     initializeVertexBuffer();
static void     initializeVertexArray();

static void
initializeProgram()
{
    static float const skFrustumScale = 1.0f;
    static float const skZNear = 0.5f;
    static float const skZFar = 3.0f;

    float theMatrix[16] = {0};
    GLuint vertexShader;
    GLuint fragmentShader;

    vertexShader = frameworkLoadShader(GL_VERTEX_SHADER, "matrix_perspective.vert");
    fragmentShader = frameworkLoadShader(GL_FRAGMENT_SHADER, "standard_colors.frag");

    sTheProgram = frameworkCreateProgram(vertexShader,
                                         fragmentShader,
                                         skShaderAttribLocations,
                                         ARRAY_COUNT(skShaderAttribLocations));

    sOffsetUniform = glGetUniformLocation(sTheProgram, "u_offset");
    sPerspectiveMatrixUniform = glGetUniformLocation(sTheProgram, "u_perspectiveMatrix");

    theMatrix[0] = skFrustumScale;
    theMatrix[5] = skFrustumScale;
    theMatrix[10] = (skZFar + skZNear) / (skZNear - skZFar);
    theMatrix[11] = -1.0f;
    theMatrix[14] = (2.0f * skZFar * skZNear) / (skZNear - skZFar);

    glUseProgram(sTheProgram);
    glUniformMatrix4fv(sPerspectiveMatrixUniform, 1, GL_FALSE, theMatrix);
    glUseProgram(0);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

static void
initializeVertexBuffer()
{
    glGenBuffers(1, &sPositionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, sPositionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skVertexData), skVertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
gltutDefaultSettingsInit(GltutDefaultSettings *pSettings)
{
    char const *name;

    name = strrchr(__FILE__, '/');
    name = (name != NULL) ? name + 1 : __FILE__;
    pSettings->windowTitle = name;
}

void
gltutReshape(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

static void
initializeVertexArray(void)
{
#ifndef GL_ES
    GLuint vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#endif
}

void
gltutPostRenderSystemInit(void)
{
    initializeProgram();
    initializeVertexBuffer();
    initializeVertexArray();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}

void
gltutDisplay(void)
{
    static size_t const skColorDataOffset = sizeof(skVertexData) / 2;
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(sTheProgram);
    glUniform2f(sOffsetUniform, 0.5f, 0.5f);

    glBindBuffer(GL_ARRAY_BUFFER, sPositionBufferObject);
    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_COLOR, 4, GL_FLOAT, GL_FALSE, 0, (void *)skColorDataOffset);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glDisableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);
    glUseProgram(0);
}
