// External libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>

// External tools
#include "../ExternalTools/MessageDebugger/MessageDebugger.h"
#include "../ExternalTools/RuntimeLogger/RuntimeLogger.h"
#include "../ExternalTools/OpenGLDebugger/OpenGlDebugger.h"

#include "Engine/Rendering/IndexBufferObject.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Rendering/VertexArrayObject.h"
#include "Engine/Rendering/VertexBufferObject.h"
#include "Engine/Rendering/Shader.h"

static constexpr bool IS_DEBUGGING_SECONDS_PAST_BETWEEN_FRAMES = false;
static constexpr bool IS_GPU_IN_DEBUG_MODE = true;

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
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, IS_GPU_IN_DEBUG_MODE);

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

    // In order to not have duplicates of the same vertices we identify each vertex with an index
    // After that we will use it to draw our geometrical form

    // GL_ELEMENT_ARRAY_BUFFER is used below to indicate the vertexBufferObjectID that we will also create below
    // contains the indices of each element in the "other" (GL_ARRAY_BUFFER) vertexBufferObjectID. 

    unsigned int geometryIndices[] =
    {
        0, 1, 2, // First triangle, down-left -> down-right -> up-right
        2, 3, 0  // Second triangle, up-right -> up-left -> down-left
    };

    // NOTE : To have documentation pass your cursor on the class name
    
    VertexBufferObject vertexBufferObject(geometryPositions, sizeof(GLfloat) * 4 * 2);
    
    VertexBufferLayoutObject vertexBufferLayoutObject;  
    vertexBufferLayoutObject.PushBack<float>(2, false);
    
    VertexArrayObject vertexArrayObject;
    vertexArrayObject.AddBuffer(vertexBufferObject, vertexBufferLayoutObject);


    // Creating the IBO (Index Buffer Object)
    // Contains indexes that say "which vertex to draw at what time" 

    IndexBufferObject indexBufferObject(geometryIndices, 6);

    Shader defaultShader("Source/Shaders/Default.shader");

    // Passing the color to the shader (to the 'u_Color' uniform variable)
    defaultShader.Bind();
    defaultShader.SetUniform4f("u_Color", 0.2f, 0.2f, 0.8f, 1.0f);

    vertexArrayObject.Unbind();
    vertexBufferObject.Unbind();
    indexBufferObject.Unbind();
    defaultShader.Unbind();


    Renderer renderer;

    float redColor = 1;
    float colorIncrement = 0.025f;
    
    // To debug framerate
    double startTime = 0;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        if (IS_DEBUGGING_SECONDS_PAST_BETWEEN_FRAMES)
            startTime = glfwGetTime();

        // Rendering

        renderer.Clear();

        // Changing the color of the drawn object
        defaultShader.Bind();
        defaultShader.SetUniform4f("u_Color", redColor, 0.2f, 0.8f, 1.0f);

        renderer.Draw(vertexArrayObject, indexBufferObject, defaultShader);
        
        // Clamping and changing the Red color value of the drawn object 
        if (redColor > 1.0f)
            colorIncrement = -0.015f;
        else if (redColor < 0.0f)
            colorIncrement =  0.015f;

        redColor += colorIncrement;
        
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        if (IS_DEBUGGING_SECONDS_PAST_BETWEEN_FRAMES)
        {
            double endTime = glfwGetTime();

            // NOTE : We don't use the MessageDebugger because we don't want to spam the log file
            std::cout << "Time past between this frame : " << endTime - startTime << '\n';
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}