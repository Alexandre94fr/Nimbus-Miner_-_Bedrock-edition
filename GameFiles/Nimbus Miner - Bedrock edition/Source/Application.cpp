// External libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// External tools
#include "../ExternalTools/MessageDebugger/MessageDebugger.h"
#include "../ExternalTools/RuntimeLogger/RuntimeLogger.h"

// TEMP
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgram
{
    std::string VertexShaderProgram;
    std::string FragmentShaderProgram;
};

static ShaderProgram ParseShader(const std::string& p_filePath) 
{
    std::ifstream stream(p_filePath);

    enum class ShaderType
    {
        None = -1,
        Vertex = 0,
        Fragment = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType shaderType = ShaderType::None;

    while (getline(stream, line))
    {
        // We use std::string::npos because if find() did not find something it return -1 (the value of npos)
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
            ss[(int)shaderType] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(const unsigned int p_type, const std::string& p_filePath)
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
        
        PRINT_ERROR_RUNTIME(true, std::string("Failed to compile the") + (p_type == GL_VERTEX_SHADER ? "vertex" : "fragment") + "shader")

        return 0;
    }

    return shaderID;
}

static unsigned int CreateShader(const std::string& p_vertexShaderCode, const std::string& p_fragmentShaderCode)
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


int main(void)
{
    RuntimeLogger::Log(
        "========================\n"
        " = New program launch = \n"
        "========================\n\n\n"
        , "", 0
    );

    MessageDebugger::TestMessageDebugger(TestOptionsEnum::AllTests);

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Nimbus miner - Bedrock edition", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Glew initialization
    if (glewInit() != GLEW_OK)
    {
        PRINT_ERROR_RUNTIME(true,
            "The function 'glewInit' has been called before the GLFW context creation "
            "(before the 'glfwMakeContextCurrent()' method call)\n"
            "The application has been stopped."
        )

        return -1;
    }

    // TEMP

    float trianglePointsPositions[6] =
    {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };
    
    // Creating a VBO (Vertex Buffer Object), (have sent the data to the graphic card)
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, trianglePointsPositions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, // The index of the attribute (example : if you want to bind the second attribute of your vertex you will pass 1)
        2, // The number of data in your attribute (example : If you want three float in your attributes, you pass 3)
        GL_FLOAT, // The type of each of your data
        GL_FALSE,
        2 * sizeof(float), // The bytes size of all attributes of this vertex
        // (example : if you have a vertex that have 3 attributes and each of them weight 12 bytes, that's mean you pass 12 * 3 (36 bytes))
        (void*)0
    );

    ShaderProgram shaderProgram = ParseShader("Source/Shaders/Default.shader");

    MessageDebugger::PrintMessage(std::string("Vertex shader :\n") + shaderProgram.VertexShaderProgram);
    MessageDebugger::PrintMessage(std::string("Fragment shader :\n") + shaderProgram.FragmentShaderProgram);

    unsigned int shader = CreateShader(shaderProgram.VertexShaderProgram, shaderProgram.FragmentShaderProgram);
    glUseProgram(shader);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
