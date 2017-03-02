#include <cassert.h>

#include "ArrayCount.h"
#include "FileUtil.h"
#include "glsys.h"

ShaderProgram::ShaderProgram()
  : m_programHandle(0)
    m_wasLinked(false)
{
    m_programHandle = glCreateProgram();
    assert(programHandle != 0);
}

ShaderProgram::~ShaderProgram()
{
    if (programHandle != 0) {
        glDeleteProgram(programHandle);
    }
}

void ShaderProgram::printShaderInfoLog(GLuint shaderHandle, const char* name)
{
    GLint logLength;
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        char* logBuffer = new char[logLength];
        GLsizei bytesCopied;
        glGetShaderInfoLog(shaderHandle, logLength, &bytesCopied, logBuffer);
        (void)fprintf(stderr, "Shader '%s' info log:\n%s\n", name, logBuffer);
        delete [] logBuffer;
    }
}

bool ShaderProgram::compileShader(ShaderProgram::ShaderType shaderType, const char* filename)
{
    static GLenum const sk_shaderTypeMap[] = {GL_VERTEX_SHADER,
                                              GL_FRAGMENT_SHADER};
    static_assert(ArrayCount(sk_shaderTypeMap) == ShaderType_Count, "Mapping mismatch");

    bool success = false;
    GLuint shaderHandle = glCreateShader(sk_shaderTypeMap[shaderType]);
    assert(shaderHandle != 0);
    if (shaderHandle != 0) {
        const GLchar* shaderCode = FileUtil::loadFileAsString(filename);
        glShaderSource(shaderHandle, 1, &shaderCode, NULL);
        delete [] shaderCode;
        glCompileShader(shaderHandle);

        GLint result;
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
        if (result == GL_TRUE) {
            success = true;
            (void)printf("Shader '%s' compilation was successful. Attaching...\n", filename);
            glAttachShader(m_programHandle, shaderHandle);
            glDeleteShader(shaderHandle);
            // Print warnings, if available
            printShaderInfoLog(shaderHandle, filename);
        } else {
            (void)fprintf(stderr, "Shader compilation failed for '%s'!\n", filename);
            printShaderInfoLog(shaderHandle, filename);
            glDeleteShader(shaderHandle);
            shaderHandle = 0;
        }
    } else {
        (void)fprintf(stderr, "Error creating shader for '%s'.\n", filename);
    }
    return success;
}

bool ShaderProgram::link()
{
    assert(!m_wasLinked);
    glLinkProgram(m_programHandle);

    GLint linkStatus;
    glGetProgramiv(m_programHandle, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_TRUE) {
        m_wasLinked = true;
        (void)printf("Shader link was successful!\n");
        printProgramInfoLog();
    } else {
        (void)fprintf(stderr, "Shader link failed!\n");
        printProgramInfoLog();
        glDeleteProgram(m_programHandle);
        m_programHandle = 0;
    }

    return m_wasLinked;
}

bool ShaderProgram::use()
{
    glUseProgram(m_programHandle);
}

void ShaderProgram::printActiveInputs()
{
    GLint numInputs;
    glGetProgramInterfaceiv(m_programHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numInputs);

    //GLint maxLength;
    //glGetProgramInterfaceiv(m_programHandle, GL_PROGRAM_INPUT, GL_MAX_NAME_LENGTH, &maxLength);

    GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};
    (void)printf("Active input resources:\n");
    for (int i = 0; i < numInputs; i++) {
        GLint propertyValues[ARRAY_COUNT(properties)];
        glGetProgramResourceiv(m_programHandle,
                               GL_PROGRAM_INPUT,
                               i,
                               ARRAY_COUNT(properties),
                               properties,
                               ARRAY_COUNT(propertyValues),
                               NULL,
                               propertyValues);
        char* nameBuffer = new char[propertyValues[0]];
        glGetProgramResourceName(m_programHandle, GL_PROGRAM_INPUT, i, propertyValues[0], NULL, nameBuffer);
        (void)printf("%-5d %s (%s)\n", propertyValues[2], nameBuffer, glsysGetTypeString(propertyValues[1]));
        delete [] nameBuffer;
    }
}

void ShaderProgram::printActiveUniforms()
{
    GLint numUniforms = 0;
    glGetProgramInterfaceiv(m_programHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
    
    if (numUniforms > 0) {
        static GLenum const properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};
        struct PropertyResults {
            GLint nameLength;
            GLint type;
            GLint location;
            GLint blockIndex;
        } results;
        static_assert(ArrayCount(properties) == sizeof(results)/sizeof(results.nameLength),
                      "PropertyResults is not the right size.");
        (void)printf("Active non-block uniforms\n");
        for (int i = 0; i < numUniforms; i++) {
            glGetProgramResourceiv(m_programHandle,
                                   GL_UNIFORM,
                                   i,
                                   ARRAY_COUNT(properties),
                                   properties,
                                   ARRAY_COUNT(properties),
                                   NULL,
                                   reinterpret_cast<GLint*>(&results));

            if (results.blockIndex == -1) {
                // Single uniform
                assert(results.nameLength > 0);
                char* nameBuffer = new char[results.nameLength];
                glGetProgramResourceName(m_programHandle, GL_UNIFORM, i, results.nameLength, NULL, nameBuffer);
                printf("location=%d name=%s (%s)\n", results.location, nameBuffer, glsysGetTypeString(results.type));
                delete [] nameBuffer;
            } 
        }
    }
}

void ShaderProgram::printActiveUniformBlocks()
{
    GLint numUniformBlocks = 0;
    glGetProgramInterfaceiv(m_programHandle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numUniformsBlocks);

    GLint numUniforms = 0;
    glGetProgramInterfaceiv(m_programHandle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numUniforms);
    
    static GLenum const properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};
    struct PropertyResults {
        GLint nameLength;
        GLint type;
        GLint location;
        GLint blockIndex;
    };
    PropertyResults* propertyResults = new PropertyResults[numUniforms];
    static_assert(ArrayCount(properties) == sizeof(propertyResults[0])/sizeof(propertyResults[0].nameLength),
                  "PropertyResults is not the right size.");

    for (int uniformIndex = 0; uniformIndex < numUniforms; uniformIndex++) {
        glGetProgramResourceiv(m_programHandle,
                               GL_UNIFORM,
                               uniformIndex,
                               ARRAY_COUNT(properties),
                               properties,
                               ARRAY_COUNT(properties),
                               NULL,
                               reinterpret_cast<GLint*>(&propertyResults[uniformIndex]));
    }

    for (int blockIndex = 0; blockIndex < numUniformBlocks; blockIndex++) {
        GLint blockBindingIndex;
        glGetActiveUniformBlockiv(m_programHandle, blockIndex, GL_UNIFORM_BLOCK_BINDING, &blockBindingIndex);
        glBindBufferBase(GL_UNIFORM_BUFFER, blockBindingIndex, m_uniformBufferHandle);

        // get the uniform block name
        GLenum blockNameProperty = GL_NAME_LENGTH;
        GLint blockNameLength;
        glGetProgramResourceiv(m_programHandle,
                               GL_UNIFORM_BLOCK,
                               blockIndex,
                               1,
                               &blockNameProperty,
                               1,
                               NULL,
                               &blockNameLength);

        char* blockNameBuffer = new char[blockNameLength];
        glGetProgramResourceName(m_programHandle, GL_UNIFORM_BLOCK, results.blockIndex, blockNameLength, NULL, blockNameBuffer);
        (void)printf("Uniform Block %s (binding index=%d)\n", blockNameBuffer, blockBindingIndex);

        // find the uniforms that are in this block, and print their info
        for (int uniformIndex = 0; uniformIndex < numUniforms; uniformIndex++) {
            PropertyResult& result = propertyResults[uniformIndex];
            if (result.blockIndex == blockIndex) {
                GLint uniformOffset;
                char* nameBuffer = new char[result.nameLength];
                glGetProgramResourceName(m_programHandle, GL_UNIFORM, i, result.nameLength, NULL, nameBuffer);
                glGetActiveUniformsiv(m_programHandle,
                                      1,
                                      &uniformIndex,
                                      GL_UNIFORM_OFFSET,
                                      &uniformOffset);
                printf("    %s (%s) (offset=%d)\n", nameBuffer, glsysGetTypeString(results.type), uniformOffset);
                delete [] nameBuffer;
            }
        }
        delete [] blockNameBuffer;
    }
    delete [] propertyResults;
}
