#include "Camera.h"

#include "MessageDebugger/MessageDebugger.h"

Camera::Camera(glm::vec3 p_spawnPosition, float p_movementSpeed, float p_rotationSensitivity)
{
    // Setting up class variables
    _position = p_spawnPosition;
    _movementSpeed = p_movementSpeed;
    _mouseSensitivity = p_rotationSensitivity;

    _worldUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);

    _yaw = -90.0f;
    _pitch = 0.0f;

    UpdateCameraDirectionVariables();
}

void Camera::ProcessKeyboardMovement(CameraMovementDirectionsEnum p_direction, float p_deltaTime)
{
    float velocity = _movementSpeed * p_deltaTime;

    switch (p_direction)
    {
    case Forward:
        _position += _frontDirection * velocity;
        break;

    case Backward:
        _position -= _frontDirection * velocity;
        break;

    case Upward:
        PRINT_ERROR_RUNTIME(true, "Upward camera movement not implemented.")
        break;

    case Downward:
        PRINT_ERROR_RUNTIME(true, "Downward camera movement not implemented.")
        break;

    case Leftward:
        _position -= _rightDirection * velocity;
        break;

    case Rightward:
        _position += _rightDirection * velocity;
        break;

    default:
        PRINT_ERROR_RUNTIME(true, "The given value for this switch case is not implemented.")
        break;
    }
}

void Camera::ProcessMouseMovement(float p_xOffset, float p_yOffset)
{
    p_xOffset *= _mouseSensitivity;
    p_yOffset *= _mouseSensitivity;

    _yaw += p_xOffset;
    _pitch += p_yOffset;

    // We clamp the vertical view to avoid camera flipping
    if (_pitch > 89.0f) _pitch = 89.0f;
    if (_pitch < -89.0f) _pitch = -89.0f;

    UpdateCameraDirectionVariables();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(_position, _position + _frontDirection, _upDirection);
}

void Camera::UpdateCameraDirectionVariables()
{
    glm::vec3 newFront = {0, 0, 0};

    // cos(yaw) * cos(pitch) determines the horizontal movement of the camera
    // A positive pitch tilts the camera leftward, while a negative pitch tilts it rightward
    // A +1 result tells us we are looking world rightward, while a negative -1 result tells us we are looking world leftward
    newFront.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    
    // sin(pitch) determines the vertical movement of the camera
    // A positive pitch tilts the camera upward, while a negative pitch tilts it downward
    // A +1 result tells us we are looking world upward, while a negative -1 result tells us we are looking world downward
    newFront.y = sin(glm::radians(_pitch));

    // sin(yaw) * cos(pitch) determines how much the camera moves in the Z direction
    // A +1 result tells us we are looking world forward, while a negative -1 result tells us we are looking world backward
    newFront.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));


    _frontDirection = newFront;

    // Compute the right direction by taking the cross product of the front direction and the world up vector.
    // This gives us a vector perpendicular to the front direction, which represents the camera's right movement direction
    _rightDirection = glm::cross(_frontDirection, _worldUpDirection);
    
    // Compute the up direction by taking the cross product of the right and front vectors
    // This ensures the up vector is perpendicular to both, maintaining a stable camera orientation
    _upDirection = glm::cross(_rightDirection, _frontDirection);
}