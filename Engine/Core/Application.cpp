#include "Application.h"

#include "Core/Log.h"

namespace zn
{
	bool Application::Init(const std::string& appName)
	{
		Log::Init();
		
		if (!m_window.Init(1200, 1200, appName))
		{
			ZN_CORE_CRITICAL("[Application::Init] Failed to initialize Window. Closing Application")
			return false;
		}

		if (!m_renderer.Init())
		{
			ZN_CORE_CRITICAL("[Application::Init] Failed to initialize Renderer. Closing Application")
			return false;
		}

		m_keyPressedConnection = EventSystem::Instance().Subscribe<KeyPressedEvent>(shared_from_this(), &Application::OnKeyPressed);
		m_windowClosedConnection = EventSystem::Instance().Subscribe<WindowClosedEvent>(shared_from_this(), &Application::OnWindowClosed);
		m_windowResizedConnection = EventSystem::Instance().Subscribe<WindowResizedEvent>(shared_from_this(), &Application::OnWindowResized);
			
		return true;
	}

	void Application::Run()
	{
		while (!m_window.ShouldClose())
		{
			m_window.PollEvents();

			// Process events ..
			
			// Calculate delta ..
			
			m_renderer.ClearScreen(0.2f, 0.3f, 0.3f, 1.0f);
			m_renderer.Render();
			
			m_window.RenderImGUI();
			m_window.SwapBuffers();
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
		return true;
	}

	bool Application::OnWindowResized(const WindowResizedEvent& e)
	{
		ZN_CORE_TRACE("Window ResizedEvent")
		return true;
	}
}
