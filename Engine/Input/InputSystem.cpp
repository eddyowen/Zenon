#include "InputSystem.hpp"

#include "Core/Assert.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace zn
{
    namespace
    {
        constexpr u16 KeyCodeToGLFW(KeyCode key)
        {
            ZN_ASSERT(key != KeyCode::UNKNOWN);
            
            return KeyCodesMapping_GLFW[static_cast<int>(key)];
        }
    }
    
    b8 InputSystem::Init(GLFWwindow* window)
    {
        if(!window)
        {
            ZN_CORE_ERROR("[InputSystem::Init] Failed to initialize InputSystem. Invalid GLFW window");
            return false;
        }

        m_window = window;
        m_keyStates.fill(false);
        m_keyStatesPrev.fill(false);

        return true;
    }

    void InputSystem::Update()
    {
        ZN_ASSERT(m_window != nullptr, "Could not update InputSystem. The system has not been initialized properly");

        // Update mouse position
        glfwGetCursorPos(m_window, &m_mouseX, &m_mouseY);

        // Update key states
        m_keyStatesPrev = m_keyStates;

        for(uSize i = 0; i < KeyCodesCount; i++)
        {
            int state = glfwGetKey(m_window, KeyCodeToGLFW(static_cast<KeyCode>(i)));
            m_keyStates[i] = state == GLFW_PRESS;
        }
    }

    b8 InputSystem::GetKeyState(KeyCode key) const
    {
        ZN_ASSERT(m_window != nullptr);
        
        int state = glfwGetKey(m_window, KeyCodeToGLFW(static_cast<KeyCode>(key)));
        return state == GLFW_PRESS ? true : false;
    }

    b8 InputSystem::IsKeyDown(KeyCode key) const
    {
        ZN_ASSERT(key != KeyCode::UNKNOWN);
        
        const u8 keyIdx = static_cast<u8>(key);
        return m_keyStates[keyIdx] && !m_keyStatesPrev[keyIdx];
    }
}