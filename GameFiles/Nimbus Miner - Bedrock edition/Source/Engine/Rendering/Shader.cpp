#include "Shader.h"

#include <GL/glew.h>

#include <fstream>
#include <string>
#include <sstream>

// External tools
#include <iostream>

#include "DebuggingConstants.h"
#include "MessageDebugger/MessageDebugger.h"

Shader::Shader(const std::string& p_filePath)
{
    _shaderFilePath = p_filePath;

    const ShaderProgram shaderProgram = ParseShader(p_filePath);
    _shaderID = CreateShader(shaderProgram.VertexShaderProgram, shaderProgram.FragmentShaderProgram);
    
}

Shader::~Shader()
{
    glDeleteProgram(_shaderID);
}


void Shader::Bind() const
{
    glUseProgram(_shaderID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

#pragma region // -=- SetUniform -=- // 

void Shader::SetUniform1i(const std::string& p_name, int p_value)
{
    glUniform1i(GetUniformLocation(p_name), p_value);
}

void Shader::SetUniform4f(const std::string& p_name, float p_v1, float p_v2, float p_v3, float p_v4)
{
    glUniform4f(GetUniformLocation(p_name), p_v1, p_v2, p_v3, p_v4);
}

#pragma endregion

ShaderProgram Shader::ParseShader(const std::string& p_filePath)
{
    std::ifstream stream(p_filePath);

    enum class ShaderType
    {
        None = -1,
        Vertex = 0,
        Fragment = 1
    };

    std::string line;
    std::stringstream shaderCode[2];
    ShaderType shaderType = ShaderType::None;

    while (getline(stream, line))
    {
        // We use 'std::string::npos' because if find() did not find something it returns -1 (the value of npos)
        if (line.find("// --") != std::string::npos)
            continue;

        if (line.size() == 0)
            continue;

        if (line.find("// SHADER") != std::string::npos)
        {
            if (line.find("VERTEX") != std::string::npos)
                shaderType = ShaderType::Vertex;

            else if (line.find("FRAGMENT") != std::string::npos)
                shaderType = ShaderType::Fragment;
        }
        else 
        {
            shaderCode[(int)shaderType] << line << '\n';
        }
    }
    
    if (IS_SHADER_LOADING_DEBUGGING_ON)
        PRINT_MESSAGE_RUNTIME("VERTEX SHADER :\n" + shaderCode[0].str() + '\n' + "FRAGMENT SHADER :\n" + shaderCode[1].str())
    
    return { shaderCode[0].str(), shaderCode[1].str() };
}

unsigned int Shader::CreateShader(const std::string& p_vertexShaderCode, const std::string& p_fragmentShaderCode)
{
    const unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, p_vertexShaderCode);
    const unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, p_fragmentShaderCode);

    const unsigned int shaderID = glCreateProgram();

    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);

    glLinkProgram(shaderID);
    glValidateProgram(shaderID);

    return shaderID;
}

unsigned int Shader::CompileShader(const unsigned int p_type, const std::string& p_filePath)
{
    unsigned int shaderID = glCreateShader(p_type);
    
    const char* charFilePath = (const char*)p_filePath.c_str();

    glShaderSource(shaderID, 1, &charFilePath, nullptr);
    glCompileShader(shaderID);

    int result;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;

        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* shaderText = static_cast<char*>(_malloca(length * sizeof(char)));

        glGetShaderInfoLog(shaderID, length, &length, &shaderText[0]);

        glDeleteShader(shaderID);
        
        PRINT_ERROR_RUNTIME(true, std::string("Failed to compile the ") + (p_type == GL_VERTEX_SHADER ? "vertex " : "fragment ") + "shader")

        return 0;
    }

    return shaderID;
}

int Shader::GetUniformLocation(const std::string& p_name)
{
    if (_uniformLocationCache.find(p_name) != _uniformLocationCache.end())
        return _uniformLocationCache[p_name];
    
    int location = glGetUniformLocation(_shaderID, p_name.c_str());

    if (location == -1)
        PRINT_WARNING_RUNTIME(true, std::string("Failed to get uniform location for '" + p_name + "'"))

    return location;
}