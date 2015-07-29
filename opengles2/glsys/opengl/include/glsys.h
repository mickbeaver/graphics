#ifndef _GLSYS_H_INCLUDED_
#define _GLSYS_H_INCLUDED_
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>
#include "glext.h"

#define GLSYS_SHADER_VERSION_STRING "#version 110\n"

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


void glsys_load_extensions(void);

#endif /* _GLSYS_H_INCLUDED_ */
