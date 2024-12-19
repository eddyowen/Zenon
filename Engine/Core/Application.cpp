#include "Application.h"

#include "Core/Log.h"
#include "Core/Signal.h"
#include "Utils/MemoryViewer.h"

#include <array>
#include <vector>

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

		m_window.OnWindowsClosed.Connect(*this, &Application::OnWindowClosed);
		m_window.OnWindowResized.Connect(*this, &Application::OnWindowResized);
		m_window.OnKeyPressed.Connect(*this, &Application::OnKeyPressed);

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

	void Application::OnKeyPressed(KeyPressedEvent& e)
	{
		ZN_CORE_TRACE(e.ToString())
	}

	void Application::OnWindowClosed(WindowClosedEvent& e)
	{
		ZN_CORE_TRACE(e.ToString())
		
		m_window.Cleanup();
		m_isRunning = false;
	}

	void Application::OnWindowResized(WindowResizedEvent& e)
	{
		ZN_CORE_TRACE(e.ToString())
	}
}