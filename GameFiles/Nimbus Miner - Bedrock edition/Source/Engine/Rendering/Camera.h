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

    float DeltaTime = 0.1f;
    
private:
    
    glm::vec3 _position;

    float _movementSpeed;
    float _rotationSensitivity;

    glm::vec3 _frontDirection;
    glm::vec3 _upDirection;
    glm::vec3 _rightDirection;
    glm::vec3 _worldUpDirection;

    // Rotation around the Z axis
    float _yaw; 

    // Rotation around the Y axis
    float _pitch;
    
public:
    
    Camera(const glm::vec3 p_spawnPosition, const float p_movementSpeed, const float p_rotationSensitivity);

    #pragma region - Getters / Setters -
    
    float GetMovementSpeed() const { return _movementSpeed; } 
    float GetRotationSensitivity() const { return _rotationSensitivity; } 
    
    void SetMovementSpeed(float p_movementSpeed);
    void SetRotationSensitivity(float p_rotationSensitivity);

    #pragma endregion
    
    void ProcessKeyboardMovement(CameraMovementDirectionsEnum p_direction);
    void ProcessMouseMovement(float p_xOffset, float p_yOffset);

    glm::mat4 GetViewMatrix() const;

    glm::vec3 GetPosition() const { return _position; }
    
private:

    void UpdateCameraDirectionVariables();
};