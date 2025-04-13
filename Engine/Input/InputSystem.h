#pragma once

#include "KeyCodes.h"

struct GLFWwindow;

namespace zn
{
    class InputSystem
    {
    public:
        struct KeyState
        {
            bool IsDown = false;
            bool WasDown = false;
        };

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

    private:
        GLFWwindow* m_window = nullptr;

        std::array<bool, KeyCodesCount> m_keyStates{};
        std::array<bool, KeyCodesCount> m_keyStatesPrev{};
    };
}