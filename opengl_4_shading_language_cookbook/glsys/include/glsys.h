#ifndef GL_SYS_H_INCLUDED
#define GL_SYS_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include <GL/gl.h>
#include "glext.h"

/*[[[cog
import cog
functions = sorted([x.strip() for x in open(input_filename).readlines() if x.strip()])
width = len('PFN' + max(functions, key=len) + 'PROC')
for function in functions:
    cog.outl('extern {0:<{width}} {1};'.format('PFN' + function.upper() + 'PROC', function, width=width))
]]]*/
extern PFNGLATTACHSHADERPROC             glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation;
extern PFNGLBINDBUFFERPROC               glBindBuffer;
extern PFNGLBINDVERTEXARRAYPROC          glBindVertexArray;
extern PFNGLBUFFERDATAPROC               glBufferData;
extern PFNGLBUFFERSUBDATAPROC            glBufferSubData;
extern PFNGLCOMPILESHADERPROC            glCompileShader;
extern PFNGLCREATEBUFFERSPROC            glCreateBuffers;
extern PFNGLCREATEPROGRAMPROC            glCreateProgram;
extern PFNGLCREATESHADERPROC             glCreateShader;
extern PFNGLCREATEVERTEXARRAYSPROC       glCreateVertexArrays;
extern PFNGLDEBUGMESSAGECALLBACKPROC     glDebugMessageCallback;
extern PFNGLDEBUGMESSAGECONTROLPROC      glDebugMessageControl;
extern PFNGLDELETEBUFFERSPROC            glDeleteBuffers;
extern PFNGLDELETEPROGRAMPROC            glDeleteProgram;
extern PFNGLDELETESHADERPROC             glDeleteShader;
extern PFNGLDELETEVERTEXARRAYSPROC       glDeleteVertexArrays;
extern PFNGLDETACHSHADERPROC             glDetachShader;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXARRAYATTRIBPROC  glEnableVertexArrayAttrib;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC               glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays;
extern PFNGLGETACTIVEATTRIBPROC          glGetActiveAttrib;
extern PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
extern PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC             glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC              glGetShaderiv;
extern PFNGLGETSTRINGIPROC               glGetStringi;
extern PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC              glLinkProgram;
extern PFNGLNAMEDBUFFERDATAPROC          glNamedBufferData;
extern PFNGLSHADERSOURCEPROC             glShaderSource;
extern PFNGLUNIFORM1FPROC                glUniform1f;
extern PFNGLUNIFORM2FPROC                glUniform2f;
extern PFNGLUNIFORM3FPROC                glUniform3f;
extern PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv;
extern PFNGLUSEPROGRAMPROC               glUseProgram;
extern PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
extern PFNGLVERTEXARRAYATTRIBFORMATPROC  glVertexArrayAttribFormat;
extern PFNGLVERTEXARRAYVERTEXBUFFERPROC  glVertexArrayVertexBuffer;
extern PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
//[[[end]]] (checksum: 8a28fb371dba599ccb63c9921afa5c40)

typedef void (*glsysFuncPtr)();
typedef glsysFuncPtr (*glsysFunctionLoader)(const char*);

// load GL function pionters (single context only!)
int glsysLoadFunctions(glsysFunctionLoader functionLoader);

// enum-to-string helper functions
const char* glsysGetDebugSeverityString(GLenum severity);
const char* glsysGetDebugSourceString(GLenum source);
const char* glsysGetDebugTypeString(GLenum type);
const char* glsysGetErrorString(GLenum error);

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
