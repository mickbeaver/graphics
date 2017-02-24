#include <assert.h>
#include <stdio.h>

#include "glsys.h"

PFNGLATTACHSHADERPROC             glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation;
PFNGLBINDBUFFERPROC               glBindBuffer;
PFNGLBINDVERTEXARRAYPROC          glBindVertexArray;
PFNGLBUFFERDATAPROC               glBufferData;
PFNGLBUFFERSUBDATAPROC            glBufferSubData;
PFNGLCOMPILESHADERPROC            glCompileShader;
PFNGLCREATEBUFFERSPROC            glCreateBuffers;
PFNGLCREATEPROGRAMPROC            glCreateProgram;
PFNGLCREATESHADERPROC             glCreateShader;
PFNGLCREATEVERTEXARRAYSPROC          glCreateVertexArrays;
PFNGLDEBUGMESSAGECALLBACKPROC     glDebugMessageCallback;
PFNGLDEBUGMESSAGECONTROLPROC      glDebugMessageControl;
PFNGLDELETEBUFFERSPROC            glDeleteBuffers;
PFNGLDELETEPROGRAMPROC            glDeleteProgram;
PFNGLDELETESHADERPROC             glDeleteShader;
PFNGLDELETEVERTEXARRAYSPROC       glDeleteVertexArrays;
PFNGLDETACHSHADERPROC             glDetachShader;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLENABLEVERTEXARRAYATTRIBPROC  glEnableVertexArrayAttrib;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
PFNGLGENBUFFERSPROC               glGenBuffers;
PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays;
PFNGLGETACTIVEATTRIBPROC          glGetActiveAttrib;
PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC             glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
PFNGLGETSHADERIVPROC              glGetShaderiv;
PFNGLGETSTRINGIPROC               glGetStringi;
PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation;
PFNGLLINKPROGRAMPROC              glLinkProgram;
PFNGLNAMEDBUFFERDATAPROC          glNamedBufferData;
PFNGLSHADERSOURCEPROC             glShaderSource;
PFNGLUNIFORM1FPROC                glUniform1f;
PFNGLUNIFORM2FPROC                glUniform2f;
PFNGLUNIFORM3FPROC                glUniform3f;
PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv;
PFNGLUSEPROGRAMPROC               glUseProgram;
PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
PFNGLVERTEXARRAYATTRIBFORMATPROC  glVertexArrayAttribFormat;
PFNGLVERTEXARRAYVERTEXBUFFERPROC  glVertexArrayVertexBuffer;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;

const char* glsysGetDebugSourceString(GLenum source)
{
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        return "debug";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return "window system";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return "shader compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        return "third party";
    case GL_DEBUG_SOURCE_APPLICATION:
        return "application";
    case GL_DEBUG_SOURCE_OTHER:
        return "other";
    }
    
    return "<unknown source>";
}

const char* glsysGetDebugTypeString(GLenum type)
{
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        return "error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return "deprecated behavior";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return "undefined behavior";
    case GL_DEBUG_TYPE_PORTABILITY:
        return "portability";
    case GL_DEBUG_TYPE_PERFORMANCE:
        return "performance";
    case GL_DEBUG_TYPE_OTHER:
        return "other";
    case GL_DEBUG_TYPE_MARKER:
        return "marker";
    case GL_DEBUG_TYPE_PUSH_GROUP:
        return "push group";
    case GL_DEBUG_TYPE_POP_GROUP:
        return "pop group";
    }
    return "<unknown type>";
}

const char* glsysGetDebugSeverityString(GLenum severity)
{
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        return "high";
    case GL_DEBUG_SEVERITY_MEDIUM:
        return "medium";
    case GL_DEBUG_SEVERITY_LOW:
        return "low";
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return "notification";
    }
    return "<unknown severity>";
}

const char* glsysGetErrorString(GLenum error)
{
    switch (error) {
    case GL_NO_ERROR:
        return "no error";
    case GL_INVALID_ENUM:
        return "invalid enum";
    case GL_INVALID_VALUE:
        return "invalid value";
    case GL_INVALID_OPERATION:
        return "invalid operation";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "invalid framebuffer operation";
    case GL_OUT_OF_MEMORY:
        return "out of memory";
    case GL_STACK_UNDERFLOW:
        return "stack underflow";
    case GL_STACK_OVERFLOW:
        return "stack overflow";
    }
    return "<unknown error>";
}

void glsysDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param)
{
    const char* sourceStr = glsysGetDebugSourceString(source);
    const char* typeStr = glsysGetDebugTypeString(type);
    const char* severityStr = glsysGetDebugSeverityString(severity);
    
    (void)printf("[%s][%s][%s](0x%08X): %s\n", sourceStr, typeStr, severityStr, id, message);
}

int glsysLoadFunctions(glsysFunctionLoader functionLoader)
{
    glAttachShader             = (PFNGLATTACHSHADERPROC)functionLoader("glAttachShader");
    glBindAttribLocation       = (PFNGLBINDATTRIBLOCATIONPROC)functionLoader("glBindAttribLocation");
    glBindBuffer               = (PFNGLBINDBUFFERPROC)functionLoader("glBindBuffer");
    glBindVertexArray          = (PFNGLBINDVERTEXARRAYPROC)functionLoader("glBindVertexArray");
    glBufferData               = (PFNGLBUFFERDATAPROC)functionLoader("glBufferData");
    glBufferSubData            = (PFNGLBUFFERSUBDATAPROC)functionLoader("glBufferSubData");
    glCompileShader            = (PFNGLCOMPILESHADERPROC)functionLoader("glCompileShader");
    glCreateBuffers            = (PFNGLCREATEBUFFERSPROC)functionLoader("glCreateBuffers");
    glCreateProgram            = (PFNGLCREATEPROGRAMPROC)functionLoader("glCreateProgram");
    glCreateShader             = (PFNGLCREATESHADERPROC)functionLoader("glCreateShader");
    glCreateVertexArrays       = (PFNGLCREATEVERTEXARRAYSPROC)functionLoader("glCreateVertexArrays");
    glDebugMessageCallback     = (PFNGLDEBUGMESSAGECALLBACKPROC)functionLoader("glDebugMessageCallback");
    glDebugMessageControl      = (PFNGLDEBUGMESSAGECONTROLPROC)functionLoader("glDebugMessageControl");
    glDeleteBuffers            = (PFNGLDELETEBUFFERSPROC)functionLoader("glDeleteBuffers");
    glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)functionLoader("glDeleteProgram");
    glDeleteShader             = (PFNGLDELETESHADERPROC)functionLoader("glDeleteShader");
    glDeleteVertexArrays       = (PFNGLDELETEVERTEXARRAYSPROC)functionLoader("glDeleteVertexArrays");
    glDetachShader             = (PFNGLDETACHSHADERPROC)functionLoader("glDetachShader");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)functionLoader("glEnableVertexAttribArray");
    glEnableVertexArrayAttrib  = (PFNGLENABLEVERTEXARRAYATTRIBPROC)functionLoader("glEnableVertexArrayAttrib");
    glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)functionLoader("glEnableVertexAttribArray");
    glGenBuffers               = (PFNGLGENBUFFERSPROC)functionLoader("glGenBuffers");
    glGenVertexArrays          = (PFNGLGENVERTEXARRAYSPROC)functionLoader("glGenVertexArrays");
    glGetActiveAttrib          = (PFNGLGETACTIVEATTRIBPROC)functionLoader("glGetActiveAttrib");
    glGetAttribLocation        = (PFNGLGETATTRIBLOCATIONPROC)functionLoader("glGetAttribLocation");
    glGetProgramInfoLog        = (PFNGLGETPROGRAMINFOLOGPROC)functionLoader("glGetProgramInfoLog");
    glGetProgramiv             = (PFNGLGETPROGRAMIVPROC)functionLoader("glGetProgramiv");
    glGetShaderInfoLog         = (PFNGLGETSHADERINFOLOGPROC)functionLoader("glGetShaderInfoLog");
    glGetShaderiv              = (PFNGLGETSHADERIVPROC)functionLoader("glGetShaderiv");
    glGetStringi               = (PFNGLGETSTRINGIPROC)functionLoader("glGetStringi");
    glGetUniformLocation       = (PFNGLGETUNIFORMLOCATIONPROC)functionLoader("glGetUniformLocation");
    glLinkProgram              = (PFNGLLINKPROGRAMPROC)functionLoader("glLinkProgram");
    glNamedBufferData          = (PFNGLNAMEDBUFFERDATAPROC)functionLoader("glNamedBufferData");
    glShaderSource             = (PFNGLSHADERSOURCEPROC)functionLoader("glShaderSource");
    glUniform1f                = (PFNGLUNIFORM1FPROC)functionLoader("glUniform1f");
    glUniform2f                = (PFNGLUNIFORM2FPROC)functionLoader("glUniform2f");
    glUniform3f                = (PFNGLUNIFORM3FPROC)functionLoader("glUniform3f");
    glUniformMatrix4fv         = (PFNGLUNIFORMMATRIX4FVPROC)functionLoader("glUniformMatrix4fv");
    glUseProgram               = (PFNGLUSEPROGRAMPROC)functionLoader("glUseProgram");
    glVertexArrayAttribBinding = (PFNGLVERTEXARRAYATTRIBBINDINGPROC)functionLoader("glVertexArrayAttribBinding");
    glVertexArrayAttribFormat  = (PFNGLVERTEXARRAYATTRIBFORMATPROC)functionLoader("glVertexArrayAttribFormat");
    glVertexArrayVertexBuffer  = (PFNGLVERTEXARRAYVERTEXBUFFERPROC)functionLoader("glVertexArrayVertexBuffer");
    glVertexAttribPointer      = (PFNGLVERTEXATTRIBPOINTERPROC)functionLoader("glVertexAttribPointer");

    return 0;
}
