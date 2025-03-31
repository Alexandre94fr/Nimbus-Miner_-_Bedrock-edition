#pragma once

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

enum CameraMovementDirectionsEnum
{
    Forward,
    Backward,

    Upward,
    Downward,

    Leftward,
    Rightward
};

class Camera
{
    
public:
    
    Camera(glm::vec3 p_spawnPosition, float p_movementSpeed, float p_rotationSensitivity);

    void ProcessKeyboardMovement(CameraMovementDirectionsEnum p_direction, float p_deltaTime);
    void ProcessMouseMovement(float p_xOffset, float p_yOffset);

    glm::mat4 GetViewMatrix() const;

    glm::vec3 GetPosition() const { return _position; }
    
private:
    
    glm::vec3 _position;

    float _movementSpeed;
    float _mouseSensitivity;

    glm::vec3 _frontDirection;
    glm::vec3 _upDirection;
    glm::vec3 _rightDirection;
    glm::vec3 _worldUpDirection;

    // Rotation around the Z axis
    float _yaw; 

    // Rotation around the Y axis
    float _pitch;

    void UpdateCameraDirectionVariables();
};