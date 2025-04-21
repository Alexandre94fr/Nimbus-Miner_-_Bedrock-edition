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
#include "Texture/Texture2DArray.h"

// Engine files (in Source\Engine\Inputs folder)
#include "Engine/Inputs/InputsDetector.h"

// Engine files (in Source\Constants)
#include "DebuggingConstants.h"
#include "ProjectConstants.h"

// Game files (in Source/Game)
#include "Game/ChunkGeneration/ChunkManager/ChunkManager.h"
#include "Game/ChunkGeneration/GreedyChunk/GreedyChunk.h"

// Camera creation
static Camera camera(CAMERA_SPAWN_POSITION, CAMERA_MOVEMENT_SPEED, CAMERA_ROTATION_SENSITIVITY);

// Mouse handling
static bool isCursorVisible = false;

void MouseCallback(GLFWwindow* p_window, const double p_xPosition, const double p_yPosition)
{
    // We cache the converted parameter variables
    const float xPosition = static_cast<float>(p_xPosition);
    const float yPosition = static_cast<float>(p_yPosition);
    
    // The following variables are in static because we don't want them to be destroyed when the scope ends
    static float lastX = xPosition;
    static float lastY = yPosition;

    const float xOffset = xPosition - lastX;
    const float yOffset = lastY - yPosition; // Inverted because the mouse's Y axis is inverted in OpenGL

    lastX = xPosition;
    lastY = yPosition;

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

    #pragma region - Program initialization -

    // Initialize the GLFW library
    // (she makes possible the window creation, input detection, handling other extensions)
    if (!glfwInit())
        return -1;

    // To tell the GPU if we are in debug mode,
    // must be after the glfwInit() and before the glfwCreateWindow() functions
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, IS_GPU_IN_DEBUG_MODE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(static_cast<int>(WINDOW_SIZE_X), static_cast<int>(WINDOW_SIZE_Y), "Nimbus miner - Bedrock edition", nullptr, nullptr);
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
    
    // NOTE : You can un-comment the code below if you need transparency

    // Telling OpenGL to manage alpha for our texture
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);

    // Telling OpenGL we don't want transparency
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Tells OpenGL to not draw counterclockwise faces, it's for optimization (the 'counterclockwise' it set by default by OpenGL)
    glEnable(GL_CULL_FACE); 

    #pragma endregion
    
    #pragma region - Inputs -
    
    // Call a function when the player mouse's position is changed
    glfwSetCursorPosCallback(window, MouseCallback);

    InputsDetector::Init(window);

    #pragma region Setting all game inputs
    
    // NOTE :
    // Using the [&] means all the variables inside the following lambda are pass as reference, that means if the variable disappear that can cause problems
    
    InputsDetector::AddCallback(GLFW_KEY_W, [&](){ camera.ProcessKeyboardMovement(CameraMovementDirectionsEnum::Forward); });
    InputsDetector::AddCallback(GLFW_KEY_S, [&](){ camera.ProcessKeyboardMovement(CameraMovementDirectionsEnum::Backward); });
    InputsDetector::AddCallback(GLFW_KEY_A, [&](){ camera.ProcessKeyboardMovement(CameraMovementDirectionsEnum::Leftward); });
    InputsDetector::AddCallback(GLFW_KEY_D, [&](){ camera.ProcessKeyboardMovement(CameraMovementDirectionsEnum::Rightward); });
    
    InputsDetector::AddCallback(GLFW_KEY_LEFT_ALT, [&]()
    {
        isCursorVisible = !isCursorVisible;
        glfwSetInputMode(window, GLFW_CURSOR, isCursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    });

    #pragma endregion
    
    #pragma endregion

    #pragma region - Creating data for our two testing faces -

    // NOTE : The big chunk of code below is there to explain how to draw two faces

    // For now that represent a square
    std::vector<Vertex> geometryVertexData =
    {
        Vertex(Vector3(-1.0f, 25.0f, 0.0f), /* 0 | down-left  */ Vector3(0.0f, 0.0f, 0.0f), Vector4(1, 1, 1, 1), Vector3(0.0f, 0.0f, 0)),
        Vertex(Vector3( 1.0f, 25.0f, 0.0f), /* 1 | down-right */ Vector3(0.0f, 0.0f, 0.0f), Vector4(1, 1, 1, 1), Vector3(1.0f, 0.0f, 0)),
        Vertex(Vector3( 1.0f, 27.0f, 0.0f), /* 2 | up-right   */ Vector3(0.0f, 0.0f, 0.0f), Vector4(1, 1, 1, 1), Vector3(1.0f, 1.0f, 0)),
         
        Vertex(Vector3(-1.0f, 27.0f, 0.0f), /* 3 | up-left    */ Vector3(0.0f, 0.0f, 0.0f), Vector4(1, 1, 1, 1), Vector3(0.0f, 1.0f, 0))

        // Vertices 2D position values (from itch other)
        // -0.5f, -0.5f
        //  0.5f, -0.5f
        //  0.5f,  0.5f
        
        // -0.5f,  0.5f
    };

    std::vector<Vertex> geometryVertexData2 =
    {
        Vertex(Vector3(3.0f, 25.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1, 1, 1, 1), Vector3(0.0f, 0.0f, 0)),
        Vertex(Vector3(5.0f, 25.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1, 1, 1, 1), Vector3(1.0f, 0.0f, 0)),
        Vertex(Vector3(5.0f, 27.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1, 1, 1, 1), Vector3(1.0f, 1.0f, 0)),

        Vertex(Vector3(3.0f, 27.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1, 1, 1, 1), Vector3(0.0f, 1.0f, 0))
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
    vertexBufferLayoutObject.PushBack<float>(3, false); // Represent the normal
    vertexBufferLayoutObject.PushBack<float>(4, false); // Represent the color
    vertexBufferLayoutObject.PushBack<float>(3, false); // Represent the texture position (UV)

    #pragma region VertexArrayObject creation

    #pragma region First rectangle

    VertexBufferObject vertexBufferObject(
        ConvertVerticesToFloatArray(geometryVertexData).data(),
        sizeof(Vertex) * static_cast<unsigned int>(geometryVertexData.size())
    );
    
    VertexArrayObject vertexArrayObject;
    vertexArrayObject.AddBuffer(vertexBufferObject, vertexBufferLayoutObject);

    #pragma endregion

    #pragma region Second rectangle

    VertexBufferObject vertexBufferObject2(
        ConvertVerticesToFloatArray(geometryVertexData2).data(),
        sizeof(Vertex) * static_cast<unsigned int>(geometryVertexData2.size())
    );

    VertexArrayObject vertexArrayObject2;
    vertexArrayObject2.AddBuffer(vertexBufferObject2, vertexBufferLayoutObject);

    #pragma endregion

    #pragma endregion

    // Creating the IBO (Index Buffer Object)
    // Contains indexes that say "which vertex to draw at what time" 
    // The '6' reprensents the number of values inside the 'geometryIndices' variable
    IndexBufferObject indexBufferObject(geometryIndices, 6);
    
    #pragma endregion

    #pragma region - Model View Projection matrix -

    glm::mat4 projectionMatrix = glm::perspective(
        (float)glm::radians(90.0f),
        WINDOW_SIZE_X / WINDOW_SIZE_Y,
        CAMERA_FRUSTUM_NEAR,
        CAMERA_FRUSTUM_FAR
    );
    projectionMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

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
    Texture texture("Resources/Textures/Un-official/MoiPanPan.png");
    int textureSlot = 0;
    
    texture.Bind(textureSlot);
    defaultShader.SetUniform1i("u_Texture", textureSlot);
    
    // - Chunk shader - //
    
    Shader chunkShader("Source/Shaders/ChunkShader.glsl");
    
    // Passing the ModelViewProjection (MVP) to the shader (to the 'u_ModelViewProjectionMatrix' uniform variable)
    chunkShader.Bind();
    chunkShader.SetUniformMat4f("u_ModelViewProjectionMatrix", modelViewProjectionMatrix);

    std::vector<std::string> texturePaths = {
        "Resources/Textures/Environment/Normals/CloudLight.png",
        "Resources/Textures/Environment/Normals/CloudNormal.png",
        "Resources/Textures/Environment/Normals/CloudDark.png",
        "Resources/Textures/Environment/Normals/CloudVeryDark.png",
        "Resources/Textures/Environment/Normals/CloudVeryVeryDark.png",
        
        "Resources/Textures/Environment/Ores/CloudHard.png",
        "Resources/Textures/Environment/Ores/CloudElectrified_ROTATED.png"
    };

    Texture2DArray texture2DArray(texturePaths);

    textureSlot = 0;
    texture2DArray.Bind(textureSlot);

    chunkShader.Bind();
    chunkShader.SetUniform1i("u_Texture2DArray", textureSlot);

    #pragma region TEST : To make the defaultShader use Texture2DArray

    // INSTRUCTIONS : To make that code below work you will need to change some code in the DefaultShader.glsl
    //                Un-comment the "// -- MULTI-TEXTURE VERSION : ..." lines and comment the one above.
    //                Keep in mind that's the Vertex who determines what texture is shown,
    //                so if you want to change the texture, change the third value of the TexturePosition.
    //
    //                Don't forget to un-comment the code below.
    //
    //                Or you can simply say that the object you want, uses the chunk's shader.
    
    // NOTE : The two zeros in Bind and SetUniform1i should be ALWAYS the same number (otherwise the square will be black)
    //texture2DArray.Bind(0);
    
    //defaultShader.Bind();
    //defaultShader.SetUniform1i("u_TextureArray", 0);

    #pragma endregion
    
    #pragma endregion 

    #pragma region - ImGui setup -

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    float cameraMovementSpeed = camera.GetMovementSpeed();
    float cameraRotationSensitivity = camera.GetRotationSensitivity();
    
    glm::vec3 objectsPositionOffset = { 0.0f, 0.0f, 0.0f };
    glm::vec4 testingQuadColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // To compute and debug framerate (deltaTime)
    double startTime = 0;
    double deltaTime = 0.01f; // Change each while loop

    #pragma endregion
    
    // - Chunk creation - //

    ChunkManager chunkManager(true, 1789, 0.015f, Vector3Int(32, 64, 32), 1, Vector2Int(10, 10), &chunkShader);
    
    // -- Game loop -- //
    
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        startTime = glfwGetTime();

        // - Inputs - //

        InputsDetector::ProcessInputs();

        // - Camera - //

        camera.DeltaTime = static_cast<float>(deltaTime);
        
        // - Rendering - //

        // Framebuffer cleaning
        Renderer::Clear();

        // ImGui setup
        ImGui_ImplGlfwGL3_NewFrame();

        // Changing TestingQuad color
        geometryVertexData[0].Color = Vector4(testingQuadColor.x, testingQuadColor.y, testingQuadColor.z, testingQuadColor.w);
        geometryVertexData[1].Color = Vector4(testingQuadColor.x, testingQuadColor.y, testingQuadColor.z, testingQuadColor.w);
        geometryVertexData[2].Color = Vector4(testingQuadColor.x, testingQuadColor.y, testingQuadColor.z, testingQuadColor.w);
        geometryVertexData[3].Color = Vector4(testingQuadColor.x, testingQuadColor.y, testingQuadColor.z, testingQuadColor.w);

        vertexBufferObject.SetData(geometryVertexData.data(), static_cast<unsigned int>(geometryVertexData.size()) * sizeof(Vertex));
        
        #pragma region - Updating the MVP matrix -

        // Changing the position of the drawn object
        modelMatrix = glm::translate(glm::mat4(1), objectsPositionOffset);
        viewMatrix = camera.GetViewMatrix();

        modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        
        defaultShader.Bind();
        defaultShader.SetUniformMat4f("u_ModelViewProjectionMatrix", modelViewProjectionMatrix);

        chunkShader.Bind();
        chunkShader.SetUniformMat4f("u_ModelViewProjectionMatrix", modelViewProjectionMatrix);

        #pragma endregion

        // - Drawing objects - //

        Renderer::Draw(vertexArrayObject, indexBufferObject, defaultShader);
        Renderer::Draw(vertexArrayObject2, indexBufferObject, defaultShader); // Second rectangle

        chunkManager.DrawChunks();

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
                ImGui::Indent();
                
                if (ImGui::CollapsingHeader("Camera modifications :"))
                {
                    ImGui::Indent();
                    
                    ImGui::Text("Movement speed :");
                    if (ImGui::DragFloat("Movement speed", &cameraMovementSpeed, 0.5f, 0.0f, 0.0f, "%.1f"))
                        camera.SetMovementSpeed(cameraMovementSpeed);

                    ImGui::Spacing();
                    ImGui::Text("Rotation sensitivity :");
                    if (ImGui::DragFloat("Rotation sensitivity", &cameraRotationSensitivity, 0.05f, 0.0f, 0.0f, "%.2f" ))
                        camera.SetRotationSensitivity(cameraRotationSensitivity);

                    ImGui::Unindent();
                }

                ImGui::Unindent();
                
                ImGui::Indent();

                if (ImGui::CollapsingHeader("Objects modifications :"))
                {
                    ImGui::Indent();
                    
                    ImGui::Text("Objects position offset :");
                    ImGui::DragFloat3("Position offset", &objectsPositionOffset.x);
                    // &objectsPositionOffset.x = the address of the table

                    ImGui::Spacing();
                    ImGui::Text("First testing quad color :");
                    ImGui::SliderFloat4("Color", &testingQuadColor.x, 0, 1);

                    ImGui::Unindent();
                }
                
                ImGui::Unindent();
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

    // -- Cleaning the program -- //

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}