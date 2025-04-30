#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace zn
{
    Camera::Camera(float aspectRatio, float fov, float nearClip, float farClip, float yaw, float pitch)
        : m_aspectRatio(aspectRatio), m_fov(fov), m_nearClip(nearClip), m_farClip(farClip), m_yaw(yaw), m_pitch(pitch)
    {
        m_worldUp = m_up;
        
        m_movementSpeed = 1.25f;
        m_mouseSensitivity = 0.03f;

        UpdateProjection();
        UpdateVectors();
    }

    void Camera::SetLastMousePosition(float x, float y)
    {
        m_lastMouseX = x;
        m_lastMouseY = y;
    }

    void Camera::ProcessKeyboard(MovementDirection direction, double deltaTime)
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

        UpdateView();
    }

    void Camera::ProccessMouseMovement(float currentMouseX, float currentMouseY)
    {
        if (m_invertYaw)
            currentMouseX *= -1.0f;
        if (m_invertPitch)
            currentMouseY *= -1.0f;

        if (m_firstMouseMovement)
        {
            m_lastMouseX = currentMouseX;
            m_lastMouseY = currentMouseY;
            m_firstMouseMovement = false;
        }
        
        float offsetX = m_lastMouseX - currentMouseX;
        float offsetY = currentMouseY - m_lastMouseY; // inverted as y-coordinate ranges from bottom to top

        m_lastMouseX = currentMouseX;
        m_lastMouseY = currentMouseY;

        m_yaw += offsetX * m_mouseSensitivity;
        m_pitch += offsetY * m_mouseSensitivity;

        m_pitch = std::min(m_pitch, 89.0f);
        m_pitch = std::max(m_pitch, -89.0f);

        UpdateVectors();
    }

    void Camera::ProcessMouseScroll(float offsetX, float offsetY)
    {
        m_fov -= offsetY;
        m_fov = std::max(m_fov, 1.0f);
        m_fov = std::min(m_fov, 45.0f);
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return m_view;
    }

    glm::mat4 Camera::GetProjection() const
    {
        return m_projection;
    }

    glm::mat4 Camera::GetViewProjectionMatrix() const
    {
        return m_projection * m_view;
    }

    void Camera::UpdateView()
    {
        m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
    }

    void Camera::UpdateProjection()
    {
        m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
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

        UpdateView();
    }
}

