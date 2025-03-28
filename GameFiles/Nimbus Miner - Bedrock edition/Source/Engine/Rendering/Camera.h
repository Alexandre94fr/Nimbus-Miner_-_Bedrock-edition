#pragma once

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

class Camera
{
    
public:
    
    Camera(glm::vec3 startPosition, float startSpeed, float startSensitivity);

    void ProcessKeyboardMovement(int direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset);

    glm::mat4 GetViewMatrix() const;

    glm::vec3 GetPosition() const { return position; }
    
private:
    
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;

    void UpdateCameraVectors();
};