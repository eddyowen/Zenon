#pragma once

#include "KeyCodes.h"

struct GLFWwindow;

namespace zn
{
    class InputSystem
    {
    public:
        InputSystem() = default;
        ~InputSystem() = default;

        InputSystem(const InputSystem& other) = delete;
        InputSystem(InputSystem&& other) noexcept = delete;

        InputSystem& operator=(const InputSystem& other) = delete;
        InputSystem& operator=(InputSystem&& other) noexcept = delete;

        bool Init(GLFWwindow* window);
        void Update();

        bool GetKeyState(KeyCode key) const;
        bool IsKeyDown(KeyCode key) const;

        double GetMouseX() const { return m_mouseX; }
        double GetMouseY() const { return m_mouseY; };

    private:
        GLFWwindow* m_window = nullptr;

        std::array<bool, KeyCodesCount> m_keyStates{};
        std::array<bool, KeyCodesCount> m_keyStatesPrev{};

        double m_mouseX = 0.0;
        double m_mouseY = 0.0;
    };
}