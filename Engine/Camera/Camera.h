#pragma once

#include "Core/Base.h"
#include "Math/Math.h"

namespace zn
{
    class Camera
    {
    public:
        enum class MovementDirection : u8
        {
            Forward,
            Backward,
            Left,
            Right,
        };

        Camera() = default;
        Camera(f32 aspectRatio, f32 fov = 45.0f, f32 nearClip = 0.1f, f32 farClip = 100.0f, f32 yaw = -90.0f, f32 pitch = 0.0f);

        ~Camera() = default;

        Camera(const Camera&) = default;
        Camera(Camera&&) = default;
        
        Camera& operator=(const Camera&) = default;
        Camera& operator=(Camera&&) = default;

        void ProcessKeyboard(MovementDirection direction, f64 deltaTime);
        void ProccessMouseMovement(f32 currentMouseX, f32 currentMouseY);
        void ProcessMouseScroll(f32 offsetX, f32 offsetY);

        void SetViewportSize(u32 width, u32 height) { m_aspectRatio  = static_cast<f32>(width) / static_cast<f32>(height); }
        void SetPosition(const glm::vec3& position) { m_position = position; UpdateVectors(); }
        void SetMovementSpeed(f32 speed) { m_movementSpeed = speed; }
        void SetMouseSensitivity(f32 sensitivity) { m_mouseSensitivity = sensitivity; }
        void SetLastMousePosition(f32 x, f32 y);

        math::m4 GetViewMatrix() const;
        math::m4 GetProjection() const;
        math::m4 GetViewProjectionMatrix() const;
        
    private:
        void UpdateView();
        void UpdateProjection();
        void UpdateVectors();
        
    private:
        math::m4 m_view;
        math::m4 m_projection;
        
        math::v3 m_position = { 0.0f, 0.0f, 0.0f };
        math::v3 m_up = { 0.0f, 1.0f, 0.0f };
        math::v3 m_forward;
        math::v3 m_right;
        math::v3 m_worldUp;

        f32 m_aspectRatio;
        f32 m_fov;
        f32 m_nearClip;
        f32 m_farClip;

        f32 m_yaw;
        f32 m_pitch;

        f32 m_movementSpeed;
        f32 m_mouseSensitivity;
        f32 m_lastMouseX = 0.0f;
        f32 m_lastMouseY = 0.0f;

        b8 m_invertYaw = true;
        b8 m_invertPitch = true;

        b8 m_firstMouseMovement = true;
    }; 
}
