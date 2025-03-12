#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../ExternalTools/ConsoleTextColorizer/ConsoleTextColorizer.h"

int main(void)
{
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
        ConsoleTextColorizer::SetConsoleTextColor(WindowsTextColorEnum::Red);
        std::cout << "ERROR ! The function 'glewInit' has been called before the GLFW context creation ";
        std::cout << "(before this method 'glfwMakeContextCurrent()' call)\n";
        ConsoleTextColorizer::ResetConsoleTextColorToDefault();

        return -1;
    }

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // TEMP
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f, 0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}