#ifndef GL_SYS_H_INCLUDED
#define GL_SYS_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "glcorearb.h"

extern PFNGLATTACHSHADERPROC             glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation;
extern PFNGLBINDBUFFERPROC               glBindBuffer;
extern PFNGLBINDVERTEXARRAYPROC          glBindVertexArray;
extern PFNGLBUFFERDATAPROC               glBufferData;
extern PFNGLBUFFERSUBDATAPROC            glBufferSubData;
extern PFNGLCOMPILESHADERPROC            glCompileShader;
extern PFNGLCREATEPROGRAMPROC            glCreateProgram;
extern PFNGLCREATESHADERPROC             glCreateShader;
extern PFNGLDELETEBUFFERSPROC            glDeleteBuffers;
extern PFNGLDELETEPROGRAMPROC            glDeleteProgram;
extern PFNGLDELETESHADERPROC             glDeleteShader;
extern PFNGLDELETEVERTEXARRAYSPROC       glDeleteVertexArrays;
extern PFNGLDETACHSHADERPROC             glDetachShader;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLDRAWARRAYSPROC               glDrawArrays;
extern PFNGLENABLEVERTEXARRAYATTRIBPROC  glEnableVertexArrayAttrib;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC               glGenBuffers;
extern PFNGLGENBUFFERSPROC               glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays;
extern PFNGLGETACTIVEATTRIBPROC          glGetActiveAttrib;
extern PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
extern PFNGLGETINTEGERVPROC              glGetIntegerv;
extern PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC             glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC              glGetShaderiv;
extern PFNGLGETSTRINGIPROC               glGetStringi;
extern PFNGLGETSTRINGPROC                glGetString;
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

typedef void (*glsysFuncPtr)();
typedef glsysFuncPtr (*glsysFunctionLoader)(const char*);

int glsysLoadFunctions(glsysFunctionLoader functionLoader);

#ifdef __cplusplus
}
#endif
#endif // GL_SYS_H_INCLUDED
