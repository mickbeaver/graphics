#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL_timer.h"
#include "framework.h"
#include "glsys.h"
#include "matrix.h"
#include "scalar.h"
#include "vector.h"

#define ARRAY_COUNT(array) (sizeof(array)/sizeof(array[0]))

#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR   1.0f, 0.0f, 0.0f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

typedef enum EVertexAttrIndex {
    VERTEX_ATTR_INDEX_POSITION,
    VERTEX_ATTR_INDEX_COLOR
} EVertexAttrIndex;

typedef void (*InstanceTransformFunc)(TmMat4 *,float);

static void     nullRotation(TmMat4 *pOut, float elapsedTime);
static void     rotateX(TmMat4 *pOut, float elapsedTime);
static void     rotateY(TmMat4 *pOut, float elapsedTime);
static void     rotateZ(TmMat4 *pOut, float elapsedTime);
static void     rotateAxis(TmMat4 *pOut, float elapsedTime);
static float    computeAngleRadians(float elapsedTime, float loopDuration);
static float    calcFrustumScale(float fovDegrees);
static void     constructInstanceMatrix(TmMat4 *pOut, InstanceTransformFunc transformFunc, float elapsedTime);
static void     initializeProgram();
static void     initializeVertexBuffer();
static void     initializeVertexArray();

static FrameworkShaderAttribLocation const skShaderAttribLocations[] = {
    {.name="a_position", .location=VERTEX_ATTR_INDEX_POSITION},
    {.name="a_color", .location=VERTEX_ATTR_INDEX_COLOR}
};

static GLuint s_theProgram;

static GLint  s_modelToCameraMatrixUnif;
static GLint  s_cameraToClipMatrixUnif;

static int const sk_numberOfVertices = 8;

static float const sk_vertexData[] = {
    +1.0f, +1.0f, +1.0f,
    -1.0f, -1.0f, +1.0f,
    -1.0f, +1.0f, -1.0f,
    +1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    +1.0f, +1.0f, -1.0f,
    +1.0f, -1.0f, +1.0f,
    -1.0f, +1.0f, +1.0f,

    GREEN_COLOR,
    BLUE_COLOR,
    RED_COLOR,
    BROWN_COLOR,

    GREEN_COLOR,
    BLUE_COLOR,
    RED_COLOR,
    BROWN_COLOR
};

static GLshort const  sk_indexData[] =
{
    0, 1, 2,
    1, 0, 3,
    2, 3, 0,
    3, 2, 1,

    5, 4, 6,
    4, 5, 7,
    7, 6, 4,
    6, 7, 5,
};

static GLuint s_vertexBufferObject;
static GLuint s_indexBufferObject;
static TmMat4 s_cameraToClipMatrix = TM_MAT4_IDENTITY_INITIALIZER;
static float s_frustumScale;

static float
calcFrustumScale(float fovDegrees)
{
    static float const sk_degreesToRadians = 3.14159f / 180.0f;
    float fovRadians = fovDegrees * sk_degreesToRadians;
    return 1.0f / tan(fovRadians / 2.0f);
}

static void
initializeProgram()
{
    static float const sk_zNear = 1.0f;
    static float const sk_zFar = 45.0f;

    GLuint vertexShader;
    GLuint fragmentShader;

    vertexShader = frameworkLoadShader(GL_VERTEX_SHADER, "pos_color_local_transform.vert");
    fragmentShader = frameworkLoadShader(GL_FRAGMENT_SHADER, "color_passthrough.frag");

    s_theProgram = frameworkCreateProgram(vertexShader,
                                         fragmentShader,
                                         skShaderAttribLocations,
                                         ARRAY_COUNT(skShaderAttribLocations));

    s_modelToCameraMatrixUnif = glGetUniformLocation(s_theProgram, "u_modelToCameraMatrix");
    s_cameraToClipMatrixUnif = glGetUniformLocation(s_theProgram, "u_cameraToClipMatrix");

    s_frustumScale = calcFrustumScale(45.0f);
    s_cameraToClipMatrix.m11 = s_frustumScale;
    s_cameraToClipMatrix.m22 = s_frustumScale;
    s_cameraToClipMatrix.m33 = (sk_zFar + sk_zNear) / (sk_zNear - sk_zFar);
    s_cameraToClipMatrix.m43 = -1.0f;
    s_cameraToClipMatrix.m34 = (2.0 * sk_zFar * sk_zNear) / (sk_zNear - sk_zFar);
    
    glUseProgram(s_theProgram);
    glUniformMatrix4fv(s_cameraToClipMatrixUnif, 1, GL_FALSE, (GLfloat *)&s_cameraToClipMatrix);
    glUseProgram(0);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

static void
initializeVertexBuffer()
{
    glGenBuffers(1, &s_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, s_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sk_vertexData), sk_vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &s_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sk_indexData), sk_indexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    s_cameraToClipMatrix.m11 = s_frustumScale * (height / (float)width);

    glUseProgram(s_theProgram);
    glUniformMatrix4fv(s_cameraToClipMatrixUnif, 1, GL_FALSE, (GLfloat *)&s_cameraToClipMatrix);
    glUseProgram(0);

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

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRangef(0.0f, 1.0f);
}

float
computeAngleRadians(float elapsedTime, float loopDuration)
{
    float const scale = 3.14159f * 2.0f / loopDuration;
    float currentTimeThroughLoop = fmodf(elapsedTime, loopDuration);

    return currentTimeThroughLoop * scale;
}

void
nullRotation(TmMat4 *pOut, float elapsedTime)
{
    (void) elapsedTime;
 
    pOut->m34 = -25.0f;
}

void
rotateX(TmMat4 *pOut, float elapsedTime)
{
    float const angleRadians = computeAngleRadians(elapsedTime, 3.0f);
    float const angleCos = cosf(angleRadians);
    float const angleSin = sinf(angleRadians);

    pOut->m22 = angleCos;
    pOut->m23 = -angleSin;
    pOut->m32 = angleSin;
    pOut->m33 = angleCos;
    
    pOut->m14 = -5.0f;
    pOut->m24 = -5.0f;
    pOut->m34 = -25.0f;
}

void
rotateY(TmMat4 *pOut, float elapsedTime)
{
    float const angleRadians = computeAngleRadians(elapsedTime, 2.0f);
    float const angleCos = cosf(angleRadians);
    float const angleSin = sinf(angleRadians);

    pOut->m11 = angleCos;
    pOut->m13 = angleSin;
    pOut->m31 = -angleSin;
    pOut->m33 = angleCos;
    
    pOut->m14 = -5.0f;
    pOut->m24 = 5.0f;
    pOut->m34 = -25.0f;
}

void
rotateZ(TmMat4 *pOut, float elapsedTime)
{
    float const angleRadians = computeAngleRadians(elapsedTime, 2.0f);
    float const angleCos = cosf(angleRadians);
    float const angleSin = sinf(angleRadians);

    pOut->m11 = angleCos;
    pOut->m12 = -angleSin;
    pOut->m21 = angleSin;
    pOut->m22 = angleCos;
    
    pOut->m14 = 5.0f;
    pOut->m24 = 5.0f;
    pOut->m34 = -25.0f;
}

void
rotateAxis(TmMat4 *pOut, float elapsedTime)
{
    float const angleRadians = computeAngleRadians(elapsedTime, 2.0f);
    float const cosine = cosf(angleRadians);
    float const oneMinusCosine = 1.0f - cosine;
    float const sine = sinf(angleRadians);

    TmVec3 axisOriginal = {1.0f, 1.0f, 1.0f};
    TmVec3 axis;

    tmVec3Normalize(&axis, &axisOriginal);

    pOut->m11 = (axis.x * axis.x) + ((1.0f - axis.x * axis.x) * cosine);
    pOut->m12 = axis.x * axis.y * oneMinusCosine - (axis.z * sine);
    pOut->m13 = axis.x * axis.z * oneMinusCosine + (axis.y * sine);

    pOut->m21 = axis.x * axis.y * oneMinusCosine + (axis.z * sine);
    pOut->m22 = (axis.y * axis.y) + ((1.0f - axis.y * axis.y) * cosine);
    pOut->m23 = axis.y * axis.z * oneMinusCosine - (axis.x * sine);

    pOut->m31 = axis.x * axis.z * oneMinusCosine - (axis.y * sine);
    pOut->m32 = axis.y * axis.z * oneMinusCosine + (axis.x * sine);
    pOut->m33 = (axis.z * axis.z) + ((1.0f - axis.z * axis.z) * cosine);

    pOut->m14 = 5.0f;
    pOut->m24 = -5.0f;
    pOut->m34 = -25.0f;
}

void
constructInstanceMatrix(TmMat4 *pOut, InstanceTransformFunc transformFunc, float elapsedTime)
{
    *pOut = TM_MAT4_IDENTITY;
    transformFunc(pOut, elapsedTime);
}

static InstanceTransformFunc s_instanceFunctions[] = {nullRotation,
                                                      rotateX,
                                                      rotateY,
                                                      rotateZ,
                                                      rotateAxis};

void
gltutDisplay(void)
{
    uintptr_t colorDataOffset;
    float elapsedTime;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(s_theProgram);

    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, s_vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indexBufferObject);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    colorDataOffset = sizeof(float) * 3 * sk_numberOfVertices;
    glVertexAttribPointer(VERTEX_ATTR_INDEX_COLOR, 4, GL_FLOAT, GL_FALSE, 0, (void *)colorDataOffset);

    elapsedTime = SDL_GetTicks() / 1000.0f;
    for (size_t i = 0; i < ARRAY_COUNT(s_instanceFunctions); ++i) {
        TmMat4 modelToCamera;

        constructInstanceMatrix(&modelToCamera, s_instanceFunctions[i], elapsedTime);
        glUniformMatrix4fv(s_modelToCameraMatrixUnif, 1, GL_FALSE, (GLfloat *)&modelToCamera);
        glDrawElements(GL_TRIANGLES, ARRAY_COUNT(sk_indexData), GL_UNSIGNED_SHORT, 0);
    }

    glDisableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glDisableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);
    glUseProgram(0);
}
