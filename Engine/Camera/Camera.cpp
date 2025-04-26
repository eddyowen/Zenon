#include "Camera.h"

#include <algorithm>
#include <ios>

#include "glm/ext/matrix_transform.hpp"

namespace zn
{
    Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
    {
        m_position = position;
        m_worldUp = up;
        m_yaw = yaw;
        m_pitch = pitch;

        m_movementSpeed = 1.25f;
        m_mouseSensitivity = 0.1f;
        m_zoom = 45.0f;

        UpdateVectors();
    }

    void Camera::ProcessKeyboard(MovementDirection direction, float deltaTime)
    {
        float velocity = m_movementSpeed * deltaTime;

        if (direction == MovementDirection::Forward)
            m_position += m_forward * velocity;
        else if (direction == MovementDirection::Backward)
            m_position -= m_forward * velocity;
        if (direction == MovementDirection::Left)
            m_position -= m_right * velocity;
        else if (direction == MovementDirection::Right)
            m_position += m_right * velocity;
    }

    void Camera::ProccessMoveMovement(float currentMouseX, float currentMouseY)
    {
        if (m_invertYaw)
            currentMouseX *= -1.0f;
        if (m_invertPitch)
            currentMouseY *= -1.0f;
        
        float offsetX = lastMouseX - currentMouseX;
        float offsetY = currentMouseY - lastMouseY; // inverted as y-coordinate ranges from bottom to top

        lastMouseX = currentMouseX;
        lastMouseY = currentMouseY;

        offsetX *= m_mouseSensitivity;
        offsetY *= m_mouseSensitivity;
        
        m_yaw += offsetX;
        m_pitch += offsetY;

        m_pitch = std::min(m_pitch, 89.0f);
        m_pitch = std::max(m_pitch, -89.0f);

        UpdateVectors();
    }

    void Camera::ProcessMouseScroll(float offsetX, float offsetY)
    {
        m_zoom -= offsetY;
        m_zoom = std::max(m_zoom, 1.0f);
        m_zoom = std::min(m_zoom, 45.0f);
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(m_position, m_position + m_forward, m_up);
    }

    void Camera::UpdateVectors()
    {
        glm::vec3 forward;
        forward.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
        forward.y = glm::sin(glm::radians(m_pitch));
        forward.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

        m_forward = glm::normalize(forward);

        m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_forward));
    }
}

