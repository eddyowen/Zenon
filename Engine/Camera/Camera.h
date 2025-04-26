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
        
        explicit Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f);
        ~Camera() = default;

        Camera(const Camera&) = default;
        Camera(Camera&&) = default;
        
        Camera& operator=(const Camera&) = default;
        Camera& operator=(Camera&&) = default;

        void ProcessKeyboard(MovementDirection direction, float deltaTime);
        void ProccessMoveMovement(float currentMouseX, float currentMouseY);
        void ProcessMouseScroll(float offsetX, float offsetY);

        void SetPosition(const glm::vec3& position) { m_position = position; }
        void SetMovementSpeed(float speed) { m_movementSpeed = speed; }
        void SetMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

        float GetZoom() const { return m_zoom; }
        glm::mat4 GetViewMatrix() const;
        
    private:
        void UpdateVectors();
    
    private:
        glm::vec3 m_position;
    
        glm::vec3 m_forward;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        float lastMouseX = 0.0f;
        float lastMouseY = 0.0f;

        float m_yaw;
        float m_pitch;

        float m_movementSpeed;
        float m_mouseSensitivity;
        float m_zoom;

        bool m_invertYaw = true;
        bool m_invertPitch = true;
    }; 
}
