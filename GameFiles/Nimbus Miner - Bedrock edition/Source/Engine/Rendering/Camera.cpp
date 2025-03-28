#include "Camera.h"

Camera::Camera(glm::vec3 startPosition, float startSpeed, float startSensitivity)
    : position(startPosition), worldUp(0.0f, 1.0f, 0.0f), yaw(-90.0f), pitch(0.0f),
      movementSpeed(startSpeed), mouseSensitivity(startSensitivity)
{
    UpdateCameraVectors();
}

void Camera::ProcessKeyboardMovement(int direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == 0) // Avant
        position += front * velocity;
    if (direction == 1) // Arrière
        position -= front * velocity;
    if (direction == 2) // Gauche
        position -= right * velocity;
    if (direction == 3) // Droite
        position += right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Limiter la vue verticale pour éviter les retournements
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
