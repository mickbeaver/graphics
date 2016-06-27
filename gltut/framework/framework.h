#ifndef GLTUT_FRAMEWORK_H
#define GLTUT_FRAMEWORK_H

#include "glsys.h"

typedef struct GltutDefaultSettings {
    int          windowWidth;
    int          windowHeight;
    const char  *windowTitle;
} GltutDefaultSettings;

#define GLTUT_DEFAULT_SETTINGS_INITIALIZER \
    {.windowWidth=500, .windowHeight=500, .windowTitle="gltut"}

typedef struct FrameworkShaderAttribLocation {
    const char  *name;
    GLuint       location;
} FrameworkShaderAttribLocation;

void    gltutPostRenderSystemInit(void);
void    gltutDisplay(void);
void    gltutReshape(int width, int height);
void    gltutDefaultSettingsInit(GltutDefaultSettings *pSettings);

GLuint  frameworkLoadShader(GLenum shaderType, const char *filename);
GLuint  frameworkCreateProgram(GLuint vertexShader,
                               GLuint fragmentShader,
                               const FrameworkShaderAttribLocation *pAttribLocations,
                               size_t attribLocationCount);

#endif // GLTUT_FRAMEWORK_H
