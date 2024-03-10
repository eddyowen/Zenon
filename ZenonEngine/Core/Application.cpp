#include "Application.h"

#include <iostream>

namespace zn
{
	bool Application::Init()
	{
		if (!m_Window.Init(1920, 1080, "Zenon Engine"))
		{
			std::cout << "Failed to initialize Window. Closing Application\n";
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
		//		std::cout << "Close\n";
		//		m_Window.Close();
		//		m_running = false;
		//	}
		//}
	}

	void Application::OnKeyPressed(KeyPressedEvent& e)
	{
		std::cout << e.ToString() << std::endl;
	}

	void Application::OnWindowClosed(WindowClosedEvent& e)
	{
		std::cout << e.ToString() << std::endl;
		m_Window.Close();
		m_IsRunning = false;
	}

	void Application::OnWindowResized(WindowResizedEvent& e)
	{
		std::cout << e.ToString() << std::endl;
	}

	void Application::OnWindowClosed_Const(WindowClosedEvent& e) const
	{
		std::cout << "Close\n";
	}

	void Application::OnEvent(Event& e)
	{
		
	}
}