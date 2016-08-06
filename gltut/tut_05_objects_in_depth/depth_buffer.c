#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL_timer.h"
#include "framework.h"
#include "glsys.h"

#define ARRAY_COUNT(array) (sizeof(array)/sizeof(array[0]))

static void     initializeProgram();
static void     initializeVertexBuffer();
static void     initializeVertexArray();

typedef enum EVertexAttrIndex {
    VERTEX_ATTR_INDEX_POSITION,
    VERTEX_ATTR_INDEX_COLOR
} EVertexAttrIndex;

static FrameworkShaderAttribLocation const skShaderAttribLocations[] = {
    {.name="a_position", .location=VERTEX_ATTR_INDEX_POSITION},
    {.name="a_color", .location=VERTEX_ATTR_INDEX_COLOR}
};

static GLuint s_theProgram;

static GLint  s_offsetUniform;
static GLint  s_perspectiveMatrixUnif;

static float s_perspectiveMatrix[16] = {0};
static float const sk_FrustumScale = 1.0f;

static int const sk_numberOfVertices = 36;

#define RIGHT_EXTENT 0.8f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT 0.20f
#define MIDDLE_EXTENT 0.0f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -1.25f
#define REAR_EXTENT -1.75f

#define GREEN_COLOR 0.75f, 0.75f, 1.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.5f, 0.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

static float const sk_vertexData[] = {
	//Object 1 positions
	LEFT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	LEFT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,

	LEFT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
	LEFT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,

	LEFT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	LEFT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	LEFT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,

	RIGHT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	RIGHT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
	RIGHT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,

	LEFT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
	LEFT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	RIGHT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
	RIGHT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,

	//Object 2 positions
	TOP_EXTENT,		RIGHT_EXTENT,	REAR_EXTENT,
	MIDDLE_EXTENT,	RIGHT_EXTENT,	FRONT_EXTENT,
	MIDDLE_EXTENT,	LEFT_EXTENT,	FRONT_EXTENT,
	TOP_EXTENT,		LEFT_EXTENT,	REAR_EXTENT,

	BOTTOM_EXTENT,	RIGHT_EXTENT,	REAR_EXTENT,
	MIDDLE_EXTENT,	RIGHT_EXTENT,	FRONT_EXTENT,
	MIDDLE_EXTENT,	LEFT_EXTENT,	FRONT_EXTENT,
	BOTTOM_EXTENT,	LEFT_EXTENT,	REAR_EXTENT,

	TOP_EXTENT,		RIGHT_EXTENT,	REAR_EXTENT,
	MIDDLE_EXTENT,	RIGHT_EXTENT,	FRONT_EXTENT,
	BOTTOM_EXTENT,	RIGHT_EXTENT,	REAR_EXTENT,
					
	TOP_EXTENT,		LEFT_EXTENT,	REAR_EXTENT,
	MIDDLE_EXTENT,	LEFT_EXTENT,	FRONT_EXTENT,
	BOTTOM_EXTENT,	LEFT_EXTENT,	REAR_EXTENT,
					
	BOTTOM_EXTENT,	RIGHT_EXTENT,	REAR_EXTENT,
	TOP_EXTENT,		RIGHT_EXTENT,	REAR_EXTENT,
	TOP_EXTENT,		LEFT_EXTENT,	REAR_EXTENT,
	BOTTOM_EXTENT,	LEFT_EXTENT,	REAR_EXTENT,

	//Object 1 colors
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,

	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,

	RED_COLOR,
	RED_COLOR,
	RED_COLOR,

	GREY_COLOR,
	GREY_COLOR,
	GREY_COLOR,

	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,

	//Object 2 colors
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,

	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,
	BROWN_COLOR,

	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,

	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,

	GREY_COLOR,
	GREY_COLOR,
	GREY_COLOR,
	GREY_COLOR,
};

static GLshort const  sk_indexData[] =
{
	0, 2, 1,
	3, 2, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	11, 13, 12,

	14, 16, 15,
	17, 16, 14,
};

static GLuint s_vertexBufferObject;
static GLuint s_indexBufferObject;

static void
initializeProgram()
{
    static float const skZNear = 0.5f;
    static float const skZFar = 3.0f;

    GLuint vertexShader;
    GLuint fragmentShader;

    vertexShader = frameworkLoadShader(GL_VERTEX_SHADER, "standard.vert");
    fragmentShader = frameworkLoadShader(GL_FRAGMENT_SHADER, "standard.frag");

    s_theProgram = frameworkCreateProgram(vertexShader,
                                         fragmentShader,
                                         skShaderAttribLocations,
                                         ARRAY_COUNT(skShaderAttribLocations));

    s_offsetUniform = glGetUniformLocation(s_theProgram, "u_offset");
    s_perspectiveMatrixUnif = glGetUniformLocation(s_theProgram, "u_perspectiveMatrix");

    s_perspectiveMatrix[0] = sk_FrustumScale;
    s_perspectiveMatrix[5] = sk_FrustumScale;
    s_perspectiveMatrix[10] = (skZFar + skZNear) / (skZNear - skZFar);
    s_perspectiveMatrix[11] = -1.0f;
    s_perspectiveMatrix[14] = (2.0f * skZFar * skZNear) / (skZNear - skZFar);

    glUseProgram(s_theProgram);
    glUniformMatrix4fv(s_perspectiveMatrixUnif, 1, GL_FALSE, s_perspectiveMatrix);
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
    s_perspectiveMatrix[0] = sk_FrustumScale / ((float)height / width);
    s_perspectiveMatrix[5] = sk_FrustumScale;

    glUseProgram(s_theProgram);
    glUniformMatrix4fv(s_perspectiveMatrixUnif, 1, GL_FALSE, s_perspectiveMatrix);
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

void
gltutDisplay(void)
{
    uintptr_t colorDataOffset;
    uintptr_t posDataOffset;
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(s_theProgram);

    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);


    // Object 1
    colorDataOffset = sizeof(float) * 3 * sk_numberOfVertices;
    glUniform3f(s_offsetUniform, 0.0f, 0.0f, 0.0f);
    glBindBuffer(GL_ARRAY_BUFFER, s_vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indexBufferObject);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_COLOR, 4, GL_FLOAT, GL_FALSE, 0, (void *)colorDataOffset);
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(sk_indexData), GL_UNSIGNED_SHORT, 0);

    // Object 2
    posDataOffset = sizeof(float) * 3 * (sk_numberOfVertices / 2);
    colorDataOffset += sizeof(float) * 4 * (sk_numberOfVertices / 2);
    glUniform3f(s_offsetUniform, 0.0f, 0.0f, -1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, s_vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indexBufferObject);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void *)posDataOffset);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_COLOR, 4, GL_FLOAT, GL_FALSE, 0, (void *)colorDataOffset);
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(sk_indexData), GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glDisableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);
    glUseProgram(0);
}
