#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgram
{
    std::string VertexShaderProgram;
    std::string FragmentShaderProgram;
};

class Shader
{

private:

    unsigned int _shaderID;

    // For debugging purpose
    std::string _shaderFilePath;

    std::unordered_map<std::string, int> _uniformLocationCache;

    
public:
    explicit Shader(const std::string& p_filePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform4f(const std::string& p_name, float p_v1, float p_v2, float p_v3, float p_v4);

private:

    ShaderProgram ParseShader(const std::string& p_filePath);
    unsigned int CreateShader(const std::string& p_vertexShaderCode, const std::string& p_fragmentShaderCode);
    unsigned int  CompileShader(const unsigned int p_type, const std::string& p_filePath);
    
    int GetUniformLocation(const std::string& p_name);
    
};
