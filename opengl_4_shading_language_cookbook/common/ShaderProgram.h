#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

class ShaderProgram {
public:
    enum ShaderType {
        ShaderType_Vertex,
        ShaderType_Fragment,
        ShaderType_Count
    };
    
    ShaderProgram();
    virtual ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    bool compileShader(ShaderType shaderType, const char* filename);
    bool link();
    bool use();

    void printActiveUniforms();
    void printActiveUniformBlocks();
    void printActiveInputs();

private:
    void printShaderInfoLog(GLuint shaderHandle, const char* name);
    void printProgramInfoLog();
    
    std::map<std::string, GLint> m_uniformLocations;
    GLuint m_programHandle;
    bool m_wasLinked;
};

#endif // SHADER_PROGRAM_H
