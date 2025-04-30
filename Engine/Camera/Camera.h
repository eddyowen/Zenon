#pragma once

#include <glm/glm.hpp>

namespace zn
{
    class Camera
    {
    public:
        enum class MovementDirection : uint8_t
        {
            Forward,
            Backward,
            Left,
            Right,
        };

        Camera() = default;
        Camera(float aspectRatio, float fov = 45.0f, float nearClip = 0.1f, float farClip = 100.0f, float yaw = -90.0f, float pitch = 0.0f);

        ~Camera() = default;

        Camera(const Camera&) = default;
        Camera(Camera&&) = default;
        
        Camera& operator=(const Camera&) = default;
        Camera& operator=(Camera&&) = default;

        void ProcessKeyboard(MovementDirection direction, double deltaTime);
        void ProccessMouseMovement(float currentMouseX, float currentMouseY);
        void ProcessMouseScroll(float offsetX, float offsetY);

        void SetViewportSize(int width, int height) { m_aspectRatio  = static_cast<float>(width) / static_cast<float>(height); }
        void SetPosition(const glm::vec3& position) { m_position = position; UpdateVectors(); }
        void SetMovementSpeed(float speed) { m_movementSpeed = speed; }
        void SetMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
        void SetLastMousePosition(float x, float y);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjection() const;
        glm::mat4 GetViewProjectionMatrix() const;
        
    private:
        void UpdateView();
        void UpdateProjection();
        void UpdateVectors();
        
    private:
        glm::mat4 m_view;
        glm::mat4 m_projection;
        
        glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };
        glm::vec3 m_forward;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        float m_aspectRatio;
        float m_fov;
        float m_nearClip;
        float m_farClip;

        float m_yaw;
        float m_pitch;

        float m_movementSpeed;
        float m_mouseSensitivity;
        float m_lastMouseX = 0.0f;
        float m_lastMouseY = 0.0f;

        bool m_invertYaw = true;
        bool m_invertPitch = true;

        bool m_firstMouseMovement = true;
    }; 
}
