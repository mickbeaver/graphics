#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include "../framework/framework.h"

typedef enum EVertexAttrIndex {
    VERTEX_ATTR_INDEX_POSITION
} EVertexAttrIndex;

static const float skVertexPositions[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
    0.75f, 0.75f, 0.0f, 1.0f,
    -0.75f, 0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f
};

static GLuint sTheProgram;
static GLuint sPositionBufferObject;
static const GLchar * const skVertexShader =
    "#version 100                 \n"
    "attribute vec4 aPosition;    \n"
    "void                         \n"
    "main()                       \n"
    "{                            \n"
    "    gl_Position = aPosition; \n"
    "}                            \n";
static const GLchar * const skFragmentShader =
    "#version 100                                     \n"
    "precision mediump float;                         \n"
    "void                                             \n"
    "main()                                           \n"
    "{                                                \n"
    "    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);     \n"
    "}                                                \n";

static void     initializeProgram();
static void     initializeVertexBuffer();
static GLuint   createShader(GLenum shaderType, const GLchar *shaderSource);
static GLuint   createProgram(GLuint vertexShader, GLuint fragmentShader);

GLuint
createShader(GLenum shaderType, const GLchar *shaderSource)
{
    GLuint shader;
    GLint status;

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);

    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        GLchar *strInfoLog;
        const char *strShaderType;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        strInfoLog = (GLchar *)malloc(infoLogLength + 1);
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
        switch (shaderType) {
        case GL_VERTEX_SHADER:
            strShaderType = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            strShaderType = "fragment";
            break;
        default:
            strShaderType = "unknown";
            break;
        }
        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        free(strInfoLog);
    }

    return shader;
}

GLuint
createProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program;
    GLint status;

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glBindAttribLocation(program, VERTEX_ATTR_INDEX_POSITION, "aPosition");
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        GLchar *strInfoLog;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        strInfoLog = (char *)malloc(infoLogLength + 1);
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        free(strInfoLog);
    }
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}

void
initializeProgram()
{
    GLuint vertexShader;
    GLuint fragmentShader;

    vertexShader = createShader(GL_VERTEX_SHADER, skVertexShader);
    fragmentShader = createShader(GL_FRAGMENT_SHADER, skFragmentShader);

    sTheProgram = createProgram(vertexShader, fragmentShader);

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
    settings->windowWidth = 640;
    settings->windowHeight = 360;
    settings->windowTitle = "tut1_exercises";
}

void
gltutReshape(int width, int height)
{
    const GLsizei aspectWidth = 16;
    const GLsizei aspectHeight = 9;
    GLsizei aspectMultiple;
    GLsizei viewportWidth;
    GLsizei viewportHeight;
    
    aspectMultiple = (width / aspectWidth);
    aspectMultiple = (height / aspectHeight) < aspectMultiple ? (height / aspectHeight) : aspectMultiple;

    if (aspectMultiple == 0) {
        viewportWidth = aspectWidth;
        viewportHeight = aspectHeight;
    } else {
        viewportWidth = aspectMultiple * aspectWidth;
        viewportHeight = aspectMultiple * aspectHeight;
    }

    GLsizei startX = (width > viewportWidth) ? (width - viewportWidth) / 2 : 0;
    GLsizei startY = (height > viewportHeight) ? (height - viewportHeight) / 2 : 0;

    glViewport(startX, startY, viewportWidth, viewportHeight);
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
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(sTheProgram);

    glBindBuffer(GL_ARRAY_BUFFER, sPositionBufferObject);
    glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
    glUseProgram(0);
}
