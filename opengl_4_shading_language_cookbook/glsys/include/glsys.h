#ifndef GL_SYS_H_INCLUDED
#define GL_SYS_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "glsys.generated.h"

// enum-to-string helper functions
const char* glsysGetDebugSeverityString(GLenum severity);
const char* glsysGetDebugSourceString(GLenum source);
const char* glsysGetDebugTypeString(GLenum type);
const char* glsysGetErrorString(GLenum error);
const char* glsysGetTypeString(GLenum error);

typedef struct GlsysDebugCallbackParam {
    bool m_shouldAssert;
} GlsysDebugCallbackParam;

#define GLSYS_DEBUG_CALLBACK_PARAM_INITIALIZER {.m_shouldAssert=false}

// a default callback that will just print a message
void glsysDebugCallback(GLenum source,
                        GLenum type,
                        GLuint id,
                        GLenum severity,
                        GLsizei length,
                        const GLchar* message,
                        const void* param);

#ifdef __cplusplus
}
#endif
#endif // GL_SYS_H_INCLUDED
