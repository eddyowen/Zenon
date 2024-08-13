#include "Application.h"

#include "Core/Log.h"
#include "Utils/MemoryViewer.h"

#include <array>
#include <vector>

namespace zn
{
	bool Application::Init()
	{
		Log::Init();
		
		if (!m_Window.Init(1200, 1200, "Zenon Engine"))
		{
			ZN_CORE_ERROR("Failed to initialize Window. Closing Application");
			return false;
		}

		m_Window.KeyPressedDelegate.Bind<&Application::OnKeyPressed>(this);
		m_Window.WindowResizedDelegate.Bind<&Application::OnWindowResized>(this);
		m_Window.WindowClosedDelegate.Bind<&Application::OnWindowClosed>(this);

		return true;
	}

	void Application::Run()
	{
		m_IsRunning = true;

		while (m_IsRunning)
		{
			ProcessEvents();
			m_Window.Update();
		}
	}

	void Application::ProcessEvents()
	{
		//EventBuffer& eventBuffer = m_Window.GetEventBuffer();
		//while (!eventBuffer.IsEmpty())
		//{
		//	std::optional<Event> event = eventBuffer.Pop();
		//	if (event->GetEventType() == EventType::WindowClose)
		//	{
		//		ZN_CORE_ERROR("Close\n";
		//		m_Window.Close();
		//		m_running = false;
		//	}
		//}
	}

	void Application::OnKeyPressed(KeyPressedEvent& e)
	{
		ZN_CORE_TRACE(e.ToString());
	}

	void Application::OnWindowClosed(WindowClosedEvent& e)
	{
		ZN_CORE_TRACE(e.ToString());
		m_Window.Close();
		m_IsRunning = false;
	}

	void Application::OnWindowResized(WindowResizedEvent& e)
	{
		ZN_CORE_TRACE(e.ToString());
	}

	void Application::OnWindowClosed_Const(WindowClosedEvent& e) const
	{
		ZN_CORE_TRACE("Close");
	}

	void Application::OnEvent(Event& e)
	{
		
	}
}