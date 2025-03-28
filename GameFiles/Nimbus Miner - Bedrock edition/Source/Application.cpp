// Language library
#include <iostream>

// External libraries (in Dependencies folder)
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw_gl3.h"

// External tools (in ExternalTools folder)
#include "MessageDebugger/MessageDebugger.h"
#include "RuntimeLogger/RuntimeLogger.h"
#include "OpenGLDebugger/OpenGlDebugger.h"

// Engine files (in Source\Engine\Rendering folder)
#include "Renderer.h"
#include "Vertex.h"
#include "IndexBufferObject.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Shader.h"
#include "Texture.h"

// Engine files (in Source\Constants)
#include "DebuggingConstants.h"
#include "ProjectConstants.h"


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
    GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "Nimbus miner - Bedrock edition", nullptr, nullptr);

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
    std::vector<Vertex> geometryVertexData =
    {
        Vertex(Vector3(-100.0f, -100.0f, 0.0f), /* 0 | down-left  */ Vector4(1, 1, 1, 1), Vector2(0.0f, 0.0f)),
        Vertex(Vector3( 100.0f, -100.0f, 0.0f), /* 1 | down-right */ Vector4(1, 1, 1, 1), Vector2(1.0f, 0.0f)),
        Vertex(Vector3( 100.0f,  100.0f, 0.0f), /* 2 | up-right   */ Vector4(1, 1, 1, 1), Vector2(1.0f, 1.0f)),
         
        Vertex(Vector3(-100.0f,  100.0f, 0.0f), /* 3 | up-left    */ Vector4(1, 1, 1, 1), Vector2(0.0f, 1.0f))

        // Vertices 2D position values (from itch other)
        // -0.5f, -0.5f
        //  0.5f, -0.5f
        //  0.5f,  0.5f
        
        // -0.5f,  0.5f
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

    // Telling OpenGL to manage alpha for our texture
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    // NOTE : To have documentation pass your cursor on the class name
    //        (if you are on Visual Studio some documentation will not be shown because it's too big)
    //        #sufferingFromSuccess

    VertexBufferObject vertexBufferObject(
        ConvertVerticesToFloatArray(geometryVertexData).data(),
        sizeof(Vertex) * geometryVertexData.size()
    );
    
    VertexBufferLayoutObject vertexBufferLayoutObject;  
    vertexBufferLayoutObject.PushBack<float>(3, false); // Represent the position
    vertexBufferLayoutObject.PushBack<float>(4, false); // Represent the color
    vertexBufferLayoutObject.PushBack<float>(2, false); // Represent the texture position (UV)
    
    VertexArrayObject vertexArrayObject;
    vertexArrayObject.AddBuffer(vertexBufferObject, vertexBufferLayoutObject);


    // Creating the IBO (Index Buffer Object)
    // Contains indexes that say "which vertex to draw at what time" 

    IndexBufferObject indexBufferObject(geometryIndices, 6);

    // Matrix
    glm::mat4 projectionMatrix = glm::ortho(-640.0f, 640.0f, -480.0f, 480.0f, -1.0f, 1.0f);
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    // NOTE : Because we are using GLM we multiply all our matrix backward (the order of matrix multiplication maters)
    
    glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
    
    Shader defaultShader("Source/Shaders/DefaultShader.glsl");

    // Passing the color to the shader (to the 'u_Color' uniform variable)
    defaultShader.Bind();
    //defaultShader.SetUniform4f("u_Color", 0.2f, 0.2f, 0.8f, 1.0f);
    //defaultShader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f); // full White
    defaultShader.SetUniformMat4f("u_ModelViewProjectionMatrix", modelViewProjectionMatrix);
    
    // Passing the texture to the shader
    Texture texture("Resources/Textures/MoiPanPan.png");
    constexpr int textureSlot = 0;
    
    texture.Bind(textureSlot);
    defaultShader.SetUniform1i("u_Texture", textureSlot);
    
    vertexArrayObject.Unbind();
    vertexBufferObject.Unbind();
    indexBufferObject.Unbind();
    defaultShader.Unbind();


    Renderer renderer;

    // ImGui setup
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    
    glm::vec3 testingRectanglePositionOffset = { 0.0f, 0.0f, 0.0f };
    glm::vec4 testingRectangleColor = { 1.0f, 1.0f, 1.0f, 1.0f };

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

        // Framebuffer cleaning
        renderer.Clear();

        // ImGui setup
        ImGui_ImplGlfwGL3_NewFrame();

        // Changing TestingRectangle color
        geometryVertexData[0].Color = Vector4(testingRectangleColor.x, testingRectangleColor.y, testingRectangleColor.z, testingRectangleColor.w);
        geometryVertexData[1].Color = Vector4(testingRectangleColor.x, testingRectangleColor.y, testingRectangleColor.z, testingRectangleColor.w);
        geometryVertexData[2].Color = Vector4(testingRectangleColor.x, testingRectangleColor.y, testingRectangleColor.z, testingRectangleColor.w);
        geometryVertexData[3].Color = Vector4(testingRectangleColor.x, testingRectangleColor.y, testingRectangleColor.z, testingRectangleColor.w);

        vertexBufferObject.SetData(geometryVertexData.data(), geometryVertexData.size() * sizeof(Vertex));
        
        // Changing the color of the drawn object
        defaultShader.Bind();
        //defaultShader.SetUniform4f("u_Color", redColor, 0.2f, 0.8f, 1.0f);

        modelMatrix = glm::translate(glm::mat4(1), testingRectanglePositionOffset);
        modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        
        defaultShader.SetUniformMat4f("u_ModelViewProjectionMatrix", modelViewProjectionMatrix);
        
        renderer.Draw(vertexArrayObject, indexBufferObject, defaultShader);
        
        // Clamping and changing the Red color value of the drawn object 
        if (redColor > 1.0f)
        {
            colorIncrement = -0.015f;
        }
        else if (redColor < 0.0f)
        {
            colorIncrement =  0.015f;
        }
        
        redColor += colorIncrement;

        #pragma region - ImGui -

        {
            ImGui::Begin("Debug UI");
            
            if (ImGui::CollapsingHeader("INFORMATION :"))
            {
                ImGui::TextWrapped("This UI can be use to debug and manipulate objects during runtime.");
                ImGui::TextWrapped("In order to not close the UI by accident, and have to relaunch the project, the window is not closable.");
                ImGui::TextWrapped("You can still minimize it by clicking on the triangle on the top left of the window.");
            }

            if (ImGui::CollapsingHeader("Debug information :"))
            {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            if (ImGui::CollapsingHeader("Object modifications :"))
            {
                ImGui::Text("Testing rectangle position offset");
                ImGui::DragFloat3("Position offset", &testingRectanglePositionOffset.x);
                // &testingRectanglePositionOffset.x = the address of the table

                ImGui::Text("Testing rectangle color :");
                ImGui::SliderFloat4("Color", &testingRectangleColor.x, 0, 1);
            }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        
        #pragma endregion 

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

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}