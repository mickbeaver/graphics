#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL_timer.h"
#include "framework.h"
#include "glsys.h"

#define ARRAY_COUNT(array) (sizeof(array)/sizeof(array[0]))

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
static GLuint sPositionBufferObject;

static void     initializeProgram();
static void     initializeVertexBuffer();
static void     initializeVertexArray();
static void     computePositionOffsets(float *pXOffset, float *pYOffset);
static void     adjustVertexData(float xOffset, float yOffset);

static void
initializeProgram()
{
    GLuint vertexShader;
    GLuint fragmentShader;

    vertexShader = frameworkLoadShader(GL_VERTEX_SHADER, "standard.vert");
    fragmentShader = frameworkLoadShader(GL_FRAGMENT_SHADER, "standard.frag");

    sTheProgram = frameworkCreateProgram(vertexShader, fragmentShader, skShaderAttribLocations, ARRAY_COUNT(skShaderAttribLocations));

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
    // empty
    (void) pSettings;
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

static void
computePositionOffsets(float *pXOffset, float *pYOffset)
{
    static float const skLoopDuration = 5.0f;
    float const skScale = 3.14159f * 2.0f / skLoopDuration;
    float elapsedTime;
    float currentTimeThroughLoop;

    elapsedTime = SDL_GetTicks() / 1000.f;
    currentTimeThroughLoop = fmodf(elapsedTime, skLoopDuration);
    *pXOffset = cosf(currentTimeThroughLoop * skScale) * 0.5f;
    *pYOffset = sinf(currentTimeThroughLoop * skScale) * 0.5f;
}

static void
adjustVertexData(float xOffset, float yOffset)
{
    float newVertexPositions[ARRAY_COUNT(skVertexPositions)];
    static_assert(sizeof(skVertexPositions) == sizeof(newVertexPositions),
                  "vertex buffer size mismatch");

    (void)memcpy(newVertexPositions, skVertexPositions, sizeof(skVertexPositions));
    for (size_t i = 0; i < ARRAY_COUNT(skVertexPositions); i += 4) {
        newVertexPositions[i] += xOffset;
        newVertexPositions[i + 1] += yOffset;
    }

    glBindBuffer(GL_ARRAY_BUFFER, sPositionBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertexPositions), newVertexPositions);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
gltutDisplay(void)
{
    float xOffset = 0.0f;
    float yOffset = 0.0f;

    computePositionOffsets(&xOffset, &yOffset);
    adjustVertexData(xOffset, yOffset);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(sTheProgram);

    glBindBuffer(GL_ARRAY_BUFFER, sPositionBufferObject);
    glEnableVertexAttribArray(VERTEX_ATTR_VS_INDEX_POSITION);
    glVertexAttribPointer(VERTEX_ATTR_VS_INDEX_POSITION, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(VERTEX_ATTR_VS_INDEX_POSITION);
    glUseProgram(0);
}
