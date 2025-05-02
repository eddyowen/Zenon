#pragma once

#include "Core/Base.h"

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

        b8 Init(GLFWwindow* window);
        void Update();

        b8 GetKeyState(KeyCode key) const;
        b8 IsKeyDown(KeyCode key) const;

        f64 GetMouseX() const { return m_mouseX; }
        f64 GetMouseY() const { return m_mouseY; }

    private:
        GLFWwindow* m_window = nullptr;

        Array<b8, KeyCodesCount> m_keyStates{};
        Array<b8, KeyCodesCount> m_keyStatesPrev{};

        f64 m_mouseX = 0.0;
        f64 m_mouseY = 0.0;
    };
}