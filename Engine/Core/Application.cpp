#include "Application.h"

#include "Core/Log.h"

namespace zn
{
	bool Application::Init()
	{
		Log::Init();
		
		if (!m_window.Init(1200, 1200, "Zenon Engine"))
		{
			ZN_CORE_ERROR("Failed to initialize Window. Closing Application")
			return false;
		}

		m_keyPressedConnection = EventSystem::Instance().Subscribe<KeyPressedEvent>(shared_from_this(), &Application::OnKeyPressed);
		m_windowClosedConnection = EventSystem::Instance().Subscribe<WindowClosedEvent>(shared_from_this(), &Application::OnWindowClosed);
		m_windowResizedConnection = EventSystem::Instance().Subscribe<WindowResizedEvent>(shared_from_this(), &Application::OnWindowResized);
			
		return true;
	}

	void Application::Run()
	{
		m_isRunning = true;

		while (m_isRunning)
		{
			m_window.Update();
		}
	}

	bool Application::OnKeyPressed(const KeyPressedEvent& e)
	{
		ZN_CORE_TRACE("KeyPressedEvent: {}. ({}) repeats", e.KeyCode,  e.RepeatCount)
		
		return true;
	}

	bool Application::OnWindowClosed(const WindowClosedEvent& e)
	{
		ZN_CORE_TRACE("Window ClosedEvent")
		
		m_window.Cleanup();
		m_isRunning = false;
		
		return true;
	}

	bool Application::OnWindowResized(const WindowResizedEvent& e)
	{
		ZN_CORE_TRACE("Window ResizedEvent")
		
		return true;
	}
}
