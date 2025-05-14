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
            return;
        }
        
        f32 offsetX = m_lastMouseX - currentMouseX;
        f32 offsetY = currentMouseY - m_lastMouseY; // inverted as y-coordinate ranges from bottom to top

        m_lastMouseX = currentMouseX;
        m_lastMouseY = currentMouseY;

        m_yaw += offsetX * m_mouseSensitivity;
        m_pitch += offsetY * m_mouseSensitivity;
        m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

        UpdateVectors();
    }

    void Camera::ProcessMouseScroll(f32 offsetX, f32 offsetY)
    {
        m_fov -= offsetY;
        m_fov = std::max(m_fov, 1.0f);
        m_fov = std::min(m_fov, 45.0f);
    }

    void Camera::LookAtTarget(const math::v3& targetLocation)
    {
        const math::v3 lookAtDir = targetLocation - m_position;
        const math::v3 forward = glm::normalize(lookAtDir);

        // Consistent pitch calculation with UpdateVectors: forward.y = sin(radians(m_pitch))
        m_pitch = glm::degrees(std::asin(forward.y));
        m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

        // Consistent yaw calculation with UpdateVectors:
        //
        // If you need to find an angle from which you know its cos and sin
        // you can use atan. atan takes and y, x components (sin, cos) and returns
        // the angle in radians. In this case, and following how m_yaw is calculated,
        // atan(forward.z, forward.x) (see Obsidian notes)W
        m_yaw = glm::degrees(glm::atan(forward.z, forward.x));
    
        UpdateVectors();
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

    void Camera::UpdateView()
    {
        m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
    }
}

