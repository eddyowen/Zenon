#include "InputSystem.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace zn
{
    bool InputSystem::Init(GLFWwindow* window)
    {
        if(!window)
        {
            ZN_CORE_ERROR("[InputSystem::Init] Failed to initialize InputSystem. Invalid GLFW window")
            return false;
        }

        m_window = window;
        m_keyStates.fill(false);
        m_keyStatesPrev.fill(false);

        return true;
    }

    void InputSystem::Update()
    {
        m_keyStatesPrev = m_keyStates;

        for(int i = 0; i < KeyCodesCount; i++)
        {
            int state = glfwGetKey(m_window, KeyCodeToGLFW(static_cast<KeyCode>(i)));
            m_keyStates[i] = state == GLFW_PRESS;
        }
    }

    bool InputSystem::GetKeyState(KeyCode key) const
    {
        int state = glfwGetKey(m_window, KeyCodeToGLFW(static_cast<KeyCode>(key)));
        return state == GLFW_PRESS ? true : false;
    }

    bool InputSystem::IsKeyDown(KeyCode key) const
    {
        ZN_CORE_ASSERT(key != KeyCode::LAST);

        const int keyIdx = static_cast<int>(key);
        return m_keyStates[keyIdx] && !m_keyStatesPrev[keyIdx];
    }
}