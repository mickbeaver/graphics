#ifndef GL_SYS_H
#define GL_SYS_H

#ifdef GLSYS_GL_ES

#include <GLES2/gl2.h>

#else

#include <GL/gl.h>
#include "./glext.h"

extern PFNGLATTACHSHADERPROC            glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC      glBindAttribLocation;
extern PFNGLBINDBUFFERPROC              glBindBuffer;
extern PFNGLBUFFERDATAPROC              glBufferData;
extern PFNGLCOMPILESHADERPROC           glCompileShader;
extern PFNGLCREATEPROGRAMPROC           glCreateProgram;
extern PFNGLCREATESHADERPROC            glCreateShader;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC              glGenBuffers;
extern PFNGLGETACTIVEATTRIBPROC         glGetActiveAttrib;
extern PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation;
extern PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC            glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC             glGetShaderiv;
extern PFNGLLINKPROGRAMPROC             glLinkProgram;
extern PFNGLSHADERSOURCEPROC            glShaderSource;
extern PFNGLUSEPROGRAMPROC              glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;

#endif

void	glsysInit(void);

#endif // GL_SYS_H
