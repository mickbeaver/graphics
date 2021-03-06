#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL_timer.h"
#include "framework.h"
#include "glsys.h"

#define ARRAY_COUNT(array) (sizeof(array)/sizeof(array[0]))
#define LOOP_DURATION (5.0f)

typedef enum EVertexAttrIndex {
    VERTEX_ATTR_VS_INDEX_POSITION
} EVertexAttrIndex;

static const FrameworkShaderAttribLocation skShaderAttribLocations[] = {
    {.name="aPosition", .location=VERTEX_ATTR_VS_INDEX_POSITION}
};

static const float skVertexPositions[] = {
    0.25f, 0.25f, 0.0f, 1.0f,
    0.25f, -0.25f, 0.0f, 1.0f,
    -0.25f, -0.25f, 0.0f, 1.0f
};

static GLuint sTheProgram;
static GLint  sTimeUniformIndex;
static GLuint sPositionBufferObject;

static void     initializeProgram();
static void     initializeVertexBuffer();
static void     initializeVertexArray();

static void
initializeProgram()
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint loopDurationUniformIndex;

    vertexShader = frameworkLoadShader(GL_VERTEX_SHADER, "calc_offset.vert");
    fragmentShader = frameworkLoadShader(GL_FRAGMENT_SHADER, "standard.frag");

    sTheProgram = frameworkCreateProgram(vertexShader, fragmentShader, skShaderAttribLocations, ARRAY_COUNT(skShaderAttribLocations));
    sTimeUniformIndex = glGetUniformLocation(sTheProgram, "uTime");
    loopDurationUniformIndex = glGetUniformLocation(sTheProgram, "uLoopDuration");

    glUseProgram(sTheProgram);
    glUniform1f(loopDurationUniformIndex, LOOP_DURATION);
    glUseProgram(0);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

static void
initializeVertexBuffer()
{
    glGenBuffers(1, &sPositionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, sPositionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skVertexPositions), skVertexPositions, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
gltutDefaultSettingsInit(GltutDefaultSettings *pSettings)
{
    pSettings->windowTitle = "vert_calc_offset.c";
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
}

void
gltutDisplay(void)
{
    float timeInSeconds;

    timeInSeconds = SDL_GetTicks() / 1000.0f;
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(sTheProgram);
    glUniform1f(sTimeUniformIndex, timeInSeconds);

    glBindBuffer(GL_ARRAY_BUFFER, sPositionBufferObject);
    glEnableVertexAttribArray(VERTEX_ATTR_VS_INDEX_POSITION);
    glVertexAttribPointer(VERTEX_ATTR_VS_INDEX_POSITION, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUniform1f(sTimeUniformIndex, timeInSeconds + LOOP_DURATION / 2.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(VERTEX_ATTR_VS_INDEX_POSITION);
    glUseProgram(0);
}
