#include "Camera.hpp"

namespace zn
{
    Camera::Camera(f32 aspectRatio, f32 fov, f32 nearClip, f32 farClip, f32 yaw, f32 pitch)
        : m_aspectRatio(aspectRatio), m_fov(fov), m_nearClip(nearClip), m_farClip(farClip), m_yaw(yaw), m_pitch(pitch)
    {
        m_worldUp = m_up;
        
        m_movementSpeed = 1.25f;
        m_mouseSensitivity = 0.03f;

        UpdateProjection();
        UpdateVectors();
    }

    void Camera::SetLastMousePosition(f32 x, f32 y)
    {
        m_lastMouseX = x;
        m_lastMouseY = y;
    }

    void Camera::ProcessKeyboard(MovementDirection direction, f64 deltaTime)
    {
        f32 velocity = m_movementSpeed * deltaTime;

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

    void Camera::ProccessMouseMovement(f32 currentMouseX, f32 currentMouseY)
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
        
        f32 offsetX = m_lastMouseX - currentMouseX;
        f32 offsetY = currentMouseY - m_lastMouseY; // inverted as y-coordinate ranges from bottom to top

        m_lastMouseX = currentMouseX;
        m_lastMouseY = currentMouseY;

        m_yaw += offsetX * m_mouseSensitivity;
        m_pitch += offsetY * m_mouseSensitivity;

        m_pitch = std::min(m_pitch, 89.0f);
        m_pitch = std::max(m_pitch, -89.0f);

        UpdateVectors();
    }

    void Camera::ProcessMouseScroll(f32 offsetX, f32 offsetY)
    {
        m_fov -= offsetY;
        m_fov = std::max(m_fov, 1.0f);
        m_fov = std::min(m_fov, 45.0f);
    }

    math::m4 Camera::GetViewMatrix() const
    {
        return m_view;
    }

    math::m4 Camera::GetProjection() const
    {
        return m_projection;
    }

    math::m4 Camera::GetViewProjectionMatrix() const
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
        math::v3 forward;
        forward.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
        forward.y = glm::sin(glm::radians(m_pitch));
        forward.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

        m_forward = glm::normalize(forward);

        m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_forward));

        UpdateView();
    }
}

