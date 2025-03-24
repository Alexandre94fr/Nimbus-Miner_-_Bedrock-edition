#pragma once

#include <GL/glew.h>

#include "MessageDebugger/MessageDebugger.h"

class OpenGlDebugger
{
    
public:

    /// <summary>
    /// This method should be use inside the glDebugMessageCallback function, complete code example below.
    ///
    /// <para> This method will print out as an error the OpenGL messages. </para>
    /// 
    /// <code>
    /// // - Code example - //
    /// 
    /// // External libraries
    /// #include <GL/glew.h>
    /// #include <GLFW/glfw3.h>
    /// 
    /// // External tools (your file paths can change depending of your organization)
    /// #include "/MessageDebugger/MessageDebugger.h" 
    /// #include "/RuntimeLogger/RuntimeLogger.h"
    /// #include "/OpenGLDebugger/OpenGlDebugger.h"
    /// 
    /// static bool IsGpuInDebugMode = true;
    /// 
    /// int main(void)
    /// {
    ///     // Initialize the GLFW library
    ///     // (she makes possible the window creation, input detection, handling other extensions)
    ///     if (!glfwInit())
    ///         return -1;
    ///     
    ///     // To tell the GPU if we are in debug mode,
    ///     // must be after the glfwInit() and before the glfwCreateWindow() functions
    ///     glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, IsGpuInDebugMode);
    ///     
    ///     // Create a windowed mode window and its OpenGL context
    ///     GLFWwindow* window = glfwCreateWindow(640, 480, "Your program", nullptr, nullptr);
    ///     
    ///     // Glew initialization
    ///     if (glewInit() != GLEW_OK)
    ///     {
    ///         PRINT_ERROR_RUNTIME(true,
    ///             "The function 'glewInit' has been called before the GLFW context creation "
    ///             "(before the 'glfwMakeContextCurrent()' method call)\n"
    ///             "The application has been stopped."
    ///         )
    ///     
    ///         return -1;
    ///     }
    ///     
    ///     glEnable(GL_DEBUG_OUTPUT); // Enables OpenGL to generate debug messages and send them to the callback (if not already enabled by default)
    ///     glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // To force OpenGL to send the error right when it appends
    ///     glDebugMessageCallback(OpenGlDebugger::PrintOpenGlError, nullptr);
    /// 
    ///     // Loop until the user closes the window
    ///     while (!glfwWindowShouldClose(window))
    ///     {
    ///         // Rendering, logic, and more...
    ///     }
    /// 
    ///     glfwDestroyWindow(window);
    ///     glfwTerminate();
    ///     
    ///     return 0;
    /// }
    /// 
    /// </code> </summary>
    static void PrintOpenGlErrors(
        const GLenum p_source,
        const GLenum p_type,
        const GLuint p_id,
        const GLenum p_severity,
        const GLsizei p_length,
        const GLchar* p_message,
        const void* p_userParam
    );

private:

    static std::string GetSourceString(const GLenum p_source);
    static std::string GetTypeString(const GLenum p_type);
    static std::string GetSeverityString(const GLenum p_severity);
    
};