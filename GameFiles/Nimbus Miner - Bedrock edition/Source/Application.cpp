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
#include "Camera.h"
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

// Camera creation
static Camera camera(glm::vec3(0, 0, 3), 1.0f, 0.1f);

static bool isCursorVisible = false;

void ProcessInput(GLFWwindow* p_window, float p_deltaTime)
{
    if (glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(CameraMovementDirectionsEnum::Forward, p_deltaTime);

    if (glfwGetKey(p_window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(CameraMovementDirectionsEnum::Backward, p_deltaTime);

    if (glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(CameraMovementDirectionsEnum::Leftward, p_deltaTime);

    if (glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(CameraMovementDirectionsEnum::Rightward, p_deltaTime);

    if (glfwGetKey(p_window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        isCursorVisible = !isCursorVisible;
        glfwSetInputMode(p_window, GLFW_CURSOR, isCursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}

// Mouse callback
void MouseCallback(GLFWwindow* p_window, double p_xPosition, double p_yPosition)
{
    // The following variables are in static because we don't want them to be destroyed when the scope ends
    static float lastX = p_xPosition;
    static float lastY = p_yPosition;

    float xOffset = p_xPosition - lastX;
    float yOffset = lastY - p_yPosition; // Inverted because the mouse's Y axis is inverted in OpenGL

    lastX = p_xPosition;
    lastY = p_yPosition;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
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
    
    // Telling OpenGL to manage alpha for our texture
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Call a function when the player mouse's position is changed
    glfwSetCursorPosCallback(window, MouseCallback);

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

    std::vector<Vertex> geometryVertexData2 =
    {
        Vertex(Vector3( 300.0f, -100.0f, 0.0f), Vector4(1, 1, 1, 1), Vector2(0.0f, 0.0f)),
        Vertex(Vector3( 500.0f, -100.0f, 0.0f), Vector4(1, 1, 1, 1), Vector2(1.0f, 0.0f)),
        Vertex(Vector3( 500.0f,  100.0f, 0.0f), Vector4(1, 1, 1, 1), Vector2(1.0f, 1.0f)),
                                         
        Vertex(Vector3( 300.0f,  100.0f, 0.0f), Vector4(1, 1, 1, 1), Vector2(0.0f, 1.0f))
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
    //        (if you are on Visual Studio some documentation will not be shown because it's too big)
    //        #sufferingFromSuccess

    VertexBufferLayoutObject vertexBufferLayoutObject;
    vertexBufferLayoutObject.PushBack<float>(3, false); // Represent the position
    vertexBufferLayoutObject.PushBack<float>(4, false); // Represent the color
    vertexBufferLayoutObject.PushBack<float>(2, false); // Represent the texture position (UV)

    #pragma region - VertexArrayObject creation -

    #pragma region First rectangle

    VertexBufferObject vertexBufferObject(
        ConvertVerticesToFloatArray(geometryVertexData).data(),
        sizeof(Vertex) * geometryVertexData.size()
    );
    
    VertexArrayObject vertexArrayObject;
    vertexArrayObject.AddBuffer(vertexBufferObject, vertexBufferLayoutObject);

    #pragma endregion

    #pragma region Second rectangle

    VertexBufferObject vertexBufferObject2(
        ConvertVerticesToFloatArray(geometryVertexData2).data(),
        sizeof(Vertex) * geometryVertexData2.size()
    );

    VertexArrayObject vertexArrayObject2;
    vertexArrayObject2.AddBuffer(vertexBufferObject2, vertexBufferLayoutObject);

    #pragma endregion

    #pragma endregion

    // Creating the IBO (Index Buffer Object)
    // Contains indexes that say "which vertex to draw at what time" 
    IndexBufferObject indexBufferObject(geometryIndices, 6);
    
    #pragma region - Model View Projection matrix -

    glm::mat4 projectionMatrix = glm::perspective(
        (float)glm::radians(90.0f),
        WINDOW_SIZE_X / WINDOW_SIZE_Y,
        CAMERA_FRUSTUM_NEAR,
        CAMERA_FRUSTUM_FAR
    );
    projectionMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1000.0f));

    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    // NOTE : Because we are using GLM we multiply all our matrix backward (the order of matrix multiplication maters)
    
    glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
    
    #pragma endregion

    #pragma region - Shader -

    Shader defaultShader("Source/Shaders/DefaultShader.glsl");

    // Passing the ModelViewProjection (MVP) to the shader (to the 'u_ModelViewProjectionMatrix' uniform variable)
    defaultShader.Bind();
    defaultShader.SetUniformMat4f("u_ModelViewProjectionMatrix", modelViewProjectionMatrix);
    
    // Passing the texture to the shader
    Texture texture("Resources/Textures/MoiPanPan.png");
    constexpr int textureSlot = 0;
    
    texture.Bind(textureSlot);
    defaultShader.SetUniform1i("u_Texture", textureSlot);
    
    #pragma endregion 

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
    double deltaTime = 0.01f; // Change itch while loop

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        if (IS_DEBUGGING_SECONDS_PAST_BETWEEN_FRAMES)
            startTime = glfwGetTime();

        // Inputs
        ProcessInput(window, deltaTime);

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
        //modelMatrix *= glm::rotate(glm::mat4(1), glm::radians(redColor * 50), glm::vec3(0.0f, 0.0f, 1.0f)); // TO TEST

        glm::mat4 newModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;


        glm::mat4 viewMatrix1 = camera.GetViewMatrix();
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix1 * modelMatrix;
        

        defaultShader.SetUniformMat4f("u_ModelViewProjectionMatrix", mvpMatrix);
        
        renderer.Draw(vertexArrayObject, indexBufferObject, defaultShader);
        renderer.Draw(vertexArrayObject2, indexBufferObject, defaultShader); // Second rectangle
        
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
                ImGui::Text("Testing rectangle position offset :");
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


        double endTime = glfwGetTime();
        deltaTime = endTime - startTime;

        if (IS_DEBUGGING_SECONDS_PAST_BETWEEN_FRAMES)
        {
            // NOTE : We don't use the MessageDebugger because we don't want to spam the log file
            std::cout << "Time past between this frame : " << deltaTime << '\n';
        }
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}