// External libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// External tools
#include "../ExternalTools/MessageDebugger/MessageDebugger.h"
#include "../ExternalTools/RuntimeLogger/RuntimeLogger.h"
#include "../ExternalTools/OpenGLDebugger/OpenGlDebugger.h"

// TEMP
#include <fstream>
#include <string>
#include <sstream>

#include "Engine/Rendering/IndexBufferObject.h"
#include "Engine/Rendering/VertexBufferObject.h"


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
        
        PRINT_ERROR_RUNTIME(true, std::string("Failed to compile the ") + (p_type == GL_VERTEX_SHADER ? "vertex " : "fragment ") + "shader")

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

static bool IsGpuInDebugMode = true;

int main(void)
{
    RuntimeLogger::Log(
        "========================\n"
        " = New program launch = \n"
        "========================\n\n\n"
        , "", 0
    );

    MessageDebugger::TestMessageDebugger(TestOptionsEnum::NoTests);

    // Initialize the GLFW library
    // (she makes possible the window creation, input detection, handling other extensions)
    if (!glfwInit())
        return -1;

    // To tell the GPU if we are in debug mode,
    // must be after the glfwInit() and before the glfwCreateWindow() functions
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, IsGpuInDebugMode);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Nimbus miner - Bedrock edition", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Make the framerate constant
    glfwSwapInterval(1);
    
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

    glEnable(GL_DEBUG_OUTPUT); // Enables OpenGL to generate debug messages and send them to the callback (if not already enabled by default)
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // To force OpenGL to send the error right when it appends
    glDebugMessageCallback(OpenGlDebugger::PrintOpenGlErrors, nullptr);
    
    // TEMP

    // For now that represent a square

    float geometryPositions[] =
    {
        -0.5f, -0.5f, // 0 | down-left
         0.5f, -0.5f, // 1 | down-right
         0.5f,  0.5f, // 2 | up-right

        -0.5f,  0.5f  // 3 | up-left
    };

    // In order to not have duplicates of the same vertices we identify each vertices with an index
    // After that we will use it to draw our geometrical form

    // GL_ELEMENT_ARRAY_BUFFER is used below to indicate the vertexBufferObjectID that we will also create below
    // contains the indices of each element in the "other" (GL_ARRAY_BUFFER) vertexBufferObjectID. 

    unsigned int geometryIndices[] =
    {
        0, 1, 2, // First triangle, down-left -> down-right -> up-right
        2, 3, 0  // Second triangle, up-right -> up-left -> down-left
    };
    
    // Creating a VAO (Vertex Array Object)
    //
    // Contains the configuration for reading data from VBOs
    // 
    // It does not contain the data itself, but records :
    // - which VBOs are used
    // - which glVertexAttribPointer are active
    // - which glEnableVertexAttribArrays are activated
    // - the Index Buffer(GL_ELEMENT_ARRAY_BUFFER) too

    // +--------------------------------+
    // |    VAO | Vertex Array Object   |
    // +--------------------------------+
    // |  
    // | +------------------------------+
    // | |  VBO | Vertex Buffer Object  |
    // | |                              |
    // | |  (GL_ARRAY_BUFFER)           |
    // | |  Contains the vertices       |
    // | |  raw data                    |
    // | +------------------------------+
    // |
    // | +------------------------------+
    // | |  IBO | Index Buffer Object   |
    // | |                              |
    // | |  (GL_ELEMENT_ARRAY_BUFFER)   |
    // | |  Contains indexes            |
    // | |  that say "which vertex      |
    // | |  to draw at what time"       |
    // | +------------------------------+
    // |
    // +--------------------------------+

    unsigned int vertexArrayObjectID;
    glGenVertexArrays(1, &vertexArrayObjectID);
    glBindVertexArray(vertexArrayObjectID);

    // Creating a VBO (Vertex Buffer Object), (have sent the data to the graphic card)
    // Contains the raw data (vertex positions, colors, normals, UVs, etc.) we want to send to the GPU

    VertexBufferObject vertexBufferObject(geometryPositions, sizeof(GLfloat) * 4 * 2);
    
    // TEMP : IN CASE THE CLASS ABSTACTION DOES NOT WORK ANYMORE
    //unsigned int vertexBufferObjectID;
    //glGenBuffers(1, &vertexBufferObjectID);
    //glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, geometryPositions, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // - The index of the attribute (example : if you want to bind the second attribute of your vertex you will pass 1)
        2,                  // - The number of data in your attribute (example : If you want three float in your attributes, you pass 3)
        GL_FLOAT,           // - The type of each of your data
        GL_FALSE,           // - Is the value of each data will be normalized by OpenGL
        2 * sizeof(float),  // - The bytes size of all attributes of this vertex
                            // (example : if you have a vertex that have 3 attributes and each of them weight 12 bytes,
                            //            that's mean you pass 12 * 3 (36 bytes))
        (void*)0            // - The byte size of this attribute
                            // (example : if you have a vertex that have 2 attributes and the first one weight 8 bytes, and the second one 4, 
                            //            when you will bind the 0 attribute will set pass (void*)8, and (void*)4 if you bind the 1 attribute)
    );
    glEnableVertexAttribArray(0);

    // The 'glEnableVertexAttribArray' function will also link* the VertexArrayObject and the VertexBufferObject
    // that means we will only need to use one line of code to bind the two ( glBindVertexArray(vertexArrayObjectID); )

    // *The link between VBO and VAO is not direct or permanent,
    // it's just that when you call glVertexAttribPointer, OpenGL notes in the active VAO :
    // "For attribute 0, I need to search in such-and-such a VBO, with such-and-such a format."


    // Creating the IBO (Index Buffer Object)
    // Contains indexes that say "which vertex to draw at what time" 

    IndexBufferObject indexBufferObject(geometryIndices, 6);
    

    // TEMP : IN CASE THE CLASS ABSTACTION DOES NOT WORK ANYMORE
    //unsigned int indexBufferObjectID;       
    //glGenBuffers(1, &indexBufferObjectID);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjectID);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), geometryIndices, GL_STATIC_DRAW);

    ShaderProgram shaderProgram = ParseShader("Source/Shaders/Default.shader");

    MessageDebugger::PrintMessage(std::string("VERTEX SHADER :\n") + shaderProgram.VertexShaderProgram);
    MessageDebugger::PrintMessage(std::string("FRAGMENT SHADER :\n") + shaderProgram.FragmentShaderProgram);

    unsigned int shader = CreateShader(shaderProgram.VertexShaderProgram, shaderProgram.FragmentShaderProgram);

    // Passing the color to the shader (to the 'u_Color' uniform variable)

    int shaderLocation = glGetUniformLocation(shader, "u_Color");

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the geometry we will draw

        glUseProgram(shader);
        glUniform4f(shaderLocation, 0.2f, 0.2f, 0.8f, 1.0f);

        glBindVertexArray(vertexArrayObjectID);
        indexBufferObject.Bind();
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjectID);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        // We use nullptr because we already bind the indexBufferObjectID before

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}