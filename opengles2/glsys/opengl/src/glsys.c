#include "SDL_video.h"
#include "glsys.h"

PFNGLATTACHSHADERPROC			glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC		glBindAttribLocation;
PFNGLBINDBUFFERPROC			glBindBuffer;
PFNGLBUFFERDATAPROC			glBufferData;
PFNGLCOMPILESHADERPROC			glCompileShader;
PFNGLCREATEPROGRAMPROC			glCreateProgram;
PFNGLCREATESHADERPROC			glCreateShader;
PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray;
PFNGLGENBUFFERSPROC			glGenBuffers;
PFNGLGETACTIVEATTRIBPROC		glGetActiveAttrib;
PFNGLGETATTRIBLOCATIONPROC		glGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC		glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC			glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC		glGetShaderInfoLog;
PFNGLGETSHADERIVPROC			glGetShaderiv;
PFNGLLINKPROGRAMPROC			glLinkProgram;
PFNGLSHADERSOURCEPROC			glShaderSource;
PFNGLUSEPROGRAMPROC			glUseProgram;
PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer;

#if defined(__GNUC__) && !defined(__clang__) // GCC only
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#else
#pragma GCC diagnostic ignored "-pedantic"
#endif
#endif // GCC only
void
glsys_load_extensions(void)
{
	glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glBindAttribLocation");
	glBindBuffer = (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData");
	glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glGenBuffers = (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers");
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)SDL_GL_GetProcAddress("glGetActiveAttrib");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glGetAttribLocation");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
	glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
        glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
}
#if defined(__GNUC__) && !defined(__clang__) // GCC only
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif // GCC only
