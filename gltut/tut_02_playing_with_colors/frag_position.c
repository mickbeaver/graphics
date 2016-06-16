#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include "../framework/framework.h"

#define ARRAY_COUNT(array) (sizeof(array)/sizeof(array[0]))

typedef enum EVertexAttrIndex {
    VERTEX_ATTR_INDEX_POSITION
} EVertexAttrIndex;

static const FrameworkShaderAttribLocation skShaderAttribLocations[] = {
    {.name="aPosition", .location=VERTEX_ATTR_INDEX_POSITION}
};

static const float skVertexPositions[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f
};

static GLuint sTheProgram;
static GLuint sPositionBufferObject;

static void     initializeProgram();
static void     initializeVertexBuffer();

void
initializeProgram()
{
    GLuint vertexShader;
    GLuint fragmentShader;

    vertexShader = frameworkLoadShader(GL_VERTEX_SHADER, "frag_position.vert");
    fragmentShader = frameworkLoadShader(GL_FRAGMENT_SHADER, "frag_position.frag");

    sTheProgram = frameworkCreateProgram(vertexShader, fragmentShader, skShaderAttribLocations, ARRAY_COUNT(skShaderAttribLocations));

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void
initializeVertexBuffer()
{
    glGenBuffers(1, &sPositionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, sPositionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skVertexPositions), skVertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
gltutDefaultSettingsInit(GltutDefaultSettings *settings)
{
    // empty
    (void) settings;
}

void
gltutReshape(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void
gltutPostRenderSystemInit(void)
{
    initializeProgram();
    initializeVertexBuffer();
}

void
gltutDisplay(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(sTheProgram);

    glBindBuffer(GL_ARRAY_BUFFER, sPositionBufferObject);
    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glUseProgram(0);
}
