#pragma once

#include <string>
#include <unordered_map>

#include "GLM/gtc/matrix_transform.hpp"

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
    Shader() = default;
    explicit Shader(const std::string& p_filePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& p_name, int p_value);
    void SetUniform4f(const std::string& p_name, float p_v1, float p_v2, float p_v3, float p_v4);
    void SetUniformMat4f(const std::string& p_name, const glm::mat4& p_matrix);

private:

    ShaderProgram ParseShader(const std::string& p_filePath);
    unsigned int CreateShader(const std::string& p_vertexShaderCode, const std::string& p_fragmentShaderCode);
    unsigned int  CompileShader(const unsigned int p_type, const std::string& p_filePath);
    
    int GetUniformLocation(const std::string& p_name);
    
};
