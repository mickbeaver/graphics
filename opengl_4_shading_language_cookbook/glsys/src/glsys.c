#include <assert.h>
#include <stdio.h>

#include "glsys.h"

/*[[[cog
import cog
functions = sorted([x.strip() for x in open(input_filename).readlines() if x.strip()])
width = len('PFN' + max(functions, key=len) + 'PROC')
for function in functions:
    cog.outl('{0:<{width}} {1};'.format('PFN' + function.upper() + 'PROC', function, width=width))
]]]*/
PFNGLATTACHSHADERPROC             glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation;
PFNGLBINDBUFFERPROC               glBindBuffer;
PFNGLBINDBUFFERBASEPROC           glBindBufferBase;
PFNGLBINDVERTEXARRAYPROC          glBindVertexArray;
PFNGLBINDVERTEXBUFFERPROC         glBindVertexBuffer;
PFNGLBUFFERDATAPROC               glBufferData;
PFNGLBUFFERSUBDATAPROC            glBufferSubData;
PFNGLCOMPILESHADERPROC            glCompileShader;
PFNGLCREATEBUFFERSPROC            glCreateBuffers;
PFNGLCREATEPROGRAMPROC            glCreateProgram;
PFNGLCREATESHADERPROC             glCreateShader;
PFNGLCREATEVERTEXARRAYSPROC       glCreateVertexArrays;
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
PFNGLGETACTIVEUNIFORMBLOCKIVPROC  glGetActiveUniformBlockiv;
PFNGLGETACTIVEUNIFORMSIVPROC      glGetActiveUniformsiv;
PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
PFNGLGETPROGRAMINTERFACEIVPROC    glGetProgramInterfaceiv;
PFNGLGETPROGRAMRESOURCENAMEPROC   glGetProgramResourceName;
PFNGLGETPROGRAMRESOURCEIVPROC     glGetProgramResourceiv;
PFNGLGETPROGRAMIVPROC             glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
PFNGLGETSHADERIVPROC              glGetShaderiv;
PFNGLGETSTRINGIPROC               glGetStringi;
PFNGLGETUNIFORMBLOCKINDEXPROC     glGetUniformBlockIndex;
PFNGLGETUNIFORMINDICESPROC        glGetUniformIndices;
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
PFNGLVERTEXARRAYELEMENTBUFFERPROC glVertexArrayElementBuffer;
PFNGLVERTEXARRAYVERTEXBUFFERPROC  glVertexArrayVertexBuffer;
PFNGLVERTEXATTRIBBINDINGPROC      glVertexAttribBinding;
PFNGLVERTEXATTRIBFORMATPROC       glVertexAttribFormat;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
//[[[end]]] (checksum: abcd9e9a2d41fea3461f2d33a1ddb0e1)

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

const char* glsysGetTypeString(GLenum type)
{
    switch (type) {
    case GL_FLOAT:
        return "float";
    case GL_FLOAT_VEC2:
        return "vec2";
    case GL_FLOAT_VEC3:
        return "vec3";
    case GL_FLOAT_VEC4:
        return "vec4";
    case GL_FLOAT_MAT2:
        return "mat2";
    case GL_FLOAT_MAT3:
        return "mat3";
    case GL_FLOAT_MAT4:
        return "mat4";
    case GL_FLOAT_MAT2x3:
        return "mat2x3";
    case GL_FLOAT_MAT2x4:
        return "mat2x4";
    case GL_FLOAT_MAT3x2:
        return "mat3x2";
    case GL_FLOAT_MAT3x4:
        return "mat3x4";
    case GL_FLOAT_MAT4x2:
        return "mat4x2";
    case GL_FLOAT_MAT4x3:
        return "mat4x3";
    case GL_INT:
        return "int";
    case GL_INT_VEC2:
        return "ivec2";
    case GL_INT_VEC3:
        return "ivec3";
    case GL_INT_VEC4:
        return "ivec4";
    case GL_BOOL:
        return "bool";
    case GL_BOOL_VEC2:
        return "bec2";
    case GL_BOOL_VEC3:
        return "bvec3";
    case GL_BOOL_VEC4:
        return "bvec4";
    case GL_UNSIGNED_INT:
        return "uint";
    case GL_UNSIGNED_INT_VEC2:
        return "uvec2";
    case GL_UNSIGNED_INT_VEC3:
        return "uvec3";
    case GL_UNSIGNED_INT_VEC4:
        return "uvec4";
    case GL_DOUBLE:
        return "double";
    case GL_DOUBLE_VEC2:
        return "dvec2";
    case GL_DOUBLE_VEC3:
        return "dvec3";
    case GL_DOUBLE_VEC4:
        return "dvec4";
    case GL_DOUBLE_MAT2:
        return "dmat2";
    case GL_DOUBLE_MAT3:
        return "dmat3";
    case GL_DOUBLE_MAT4:
        return "dmat4";
    case GL_DOUBLE_MAT2x3:
        return "dmat2x3";
    case GL_DOUBLE_MAT2x4:
        return "dmat2x4";
    case GL_DOUBLE_MAT3x2:
        return "dmat3x2";
    case GL_DOUBLE_MAT3x4:
        return "dmat3x4";
    case GL_DOUBLE_MAT4x2:
        return "dmat4x2";
    case GL_DOUBLE_MAT4x3:
        return "dmat4x3";
    case GL_SAMPLER_1D:
        return "sampler1D";
    case GL_SAMPLER_2D:
        return "sampler2D";
    case GL_SAMPLER_3D:
        return "sampler3D";
    case GL_SAMPLER_CUBE:
        return "samplerCube";
    case GL_SAMPLER_1D_SHADOW:
        return "sampler1DShadow";
    case GL_SAMPLER_2D_SHADOW:
        return "sampler2DShadow";
    case GL_SAMPLER_1D_ARRAY:
        return "sampler1DArray";
    case GL_SAMPLER_2D_ARRAY:
        return "sampler2DArray";
    case GL_SAMPLER_1D_ARRAY_SHADOW:
        return "sampler1DArrayShadow";
    case GL_SAMPLER_2D_ARRAY_SHADOW:
        return "sampler2DArrayShadow";
    case GL_SAMPLER_CUBE_SHADOW:
        return "samplerCubeShadow";
    }
    return "<unknown type>";
}

void glsysDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param)
{
    const char* sourceStr = glsysGetDebugSourceString(source);
    const char* typeStr = glsysGetDebugTypeString(type);
    const char* severityStr = glsysGetDebugSeverityString(severity);
    
    (void)printf("[%s][%s][%s](0x%08X): %s\n", sourceStr, typeStr, severityStr, id, message);
    if (param != NULL) {
        const GlsysDebugCallbackParam* callbackParam = (const GlsysDebugCallbackParam*)param;
        assert(callbackParam->m_shouldAssert == false);
    }
}

int glsysLoadFunctions(glsysFunctionLoader functionLoader)
{
    /*[[[cog
    import cog
    functions = sorted([x.strip() for x in open(input_filename).readlines() if x.strip()])
    width = len(max(functions, key=len))
    for function in functions:
        cog.outl('{0:<{width}} = ({1})functionLoader("{0}");'.format(function,
                                                                     'PFN' + function.upper() + 'PROC',
                                                                     width=width))
    #for function in functions:
    #    cog.outl('assert({0:<{width}} != NULL);'.format(function, width=width));
    ]]]*/
    glAttachShader             = (PFNGLATTACHSHADERPROC)functionLoader("glAttachShader");
    glBindAttribLocation       = (PFNGLBINDATTRIBLOCATIONPROC)functionLoader("glBindAttribLocation");
    glBindBuffer               = (PFNGLBINDBUFFERPROC)functionLoader("glBindBuffer");
    glBindBufferBase           = (PFNGLBINDBUFFERBASEPROC)functionLoader("glBindBufferBase");
    glBindVertexArray          = (PFNGLBINDVERTEXARRAYPROC)functionLoader("glBindVertexArray");
    glBindVertexBuffer         = (PFNGLBINDVERTEXBUFFERPROC)functionLoader("glBindVertexBuffer");
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
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)functionLoader("glDisableVertexAttribArray");
    glEnableVertexArrayAttrib  = (PFNGLENABLEVERTEXARRAYATTRIBPROC)functionLoader("glEnableVertexArrayAttrib");
    glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)functionLoader("glEnableVertexAttribArray");
    glGenBuffers               = (PFNGLGENBUFFERSPROC)functionLoader("glGenBuffers");
    glGenVertexArrays          = (PFNGLGENVERTEXARRAYSPROC)functionLoader("glGenVertexArrays");
    glGetActiveAttrib          = (PFNGLGETACTIVEATTRIBPROC)functionLoader("glGetActiveAttrib");
    glGetActiveUniformBlockiv  = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)functionLoader("glGetActiveUniformBlockiv");
    glGetActiveUniformsiv      = (PFNGLGETACTIVEUNIFORMSIVPROC)functionLoader("glGetActiveUniformsiv");
    glGetAttribLocation        = (PFNGLGETATTRIBLOCATIONPROC)functionLoader("glGetAttribLocation");
    glGetProgramInfoLog        = (PFNGLGETPROGRAMINFOLOGPROC)functionLoader("glGetProgramInfoLog");
    glGetProgramInterfaceiv    = (PFNGLGETPROGRAMINTERFACEIVPROC)functionLoader("glGetProgramInterfaceiv");
    glGetProgramResourceName   = (PFNGLGETPROGRAMRESOURCENAMEPROC)functionLoader("glGetProgramResourceName");
    glGetProgramResourceiv     = (PFNGLGETPROGRAMRESOURCEIVPROC)functionLoader("glGetProgramResourceiv");
    glGetProgramiv             = (PFNGLGETPROGRAMIVPROC)functionLoader("glGetProgramiv");
    glGetShaderInfoLog         = (PFNGLGETSHADERINFOLOGPROC)functionLoader("glGetShaderInfoLog");
    glGetShaderiv              = (PFNGLGETSHADERIVPROC)functionLoader("glGetShaderiv");
    glGetStringi               = (PFNGLGETSTRINGIPROC)functionLoader("glGetStringi");
    glGetUniformBlockIndex     = (PFNGLGETUNIFORMBLOCKINDEXPROC)functionLoader("glGetUniformBlockIndex");
    glGetUniformIndices        = (PFNGLGETUNIFORMINDICESPROC)functionLoader("glGetUniformIndices");
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
    glVertexArrayElementBuffer = (PFNGLVERTEXARRAYELEMENTBUFFERPROC)functionLoader("glVertexArrayElementBuffer");
    glVertexArrayVertexBuffer  = (PFNGLVERTEXARRAYVERTEXBUFFERPROC)functionLoader("glVertexArrayVertexBuffer");
    glVertexAttribBinding      = (PFNGLVERTEXATTRIBBINDINGPROC)functionLoader("glVertexAttribBinding");
    glVertexAttribFormat       = (PFNGLVERTEXATTRIBFORMATPROC)functionLoader("glVertexAttribFormat");
    glVertexAttribPointer      = (PFNGLVERTEXATTRIBPOINTERPROC)functionLoader("glVertexAttribPointer");
    //[[[end]]] (checksum: e378da537ee818b4597296b2285174ce)

    return 0;
}
