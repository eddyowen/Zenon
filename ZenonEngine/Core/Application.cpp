#include "Application.h"

#include <iostream>

namespace zn
{
	bool Application::Init()
	{
		if (!m_window.Init(1920, 1080, "Zenon Engine"))
		{
			std::cout << "Failed to initialize Window. Closing Application\n";
			return false;
		}

		m_window.KeyPressedDelegate.Bind<&Application::OnKeyPressed>(this);
		m_window.WindowClosedDelegate.Bind<&Application::OnWindowClosed>(this);

		return true;
	}

	void Application::Run()
	{
		m_running = true;

		while (m_running)
		{
			ProcessEvents();
			m_window.Update();
		}
	}

	void Application::ProcessEvents()
	{
		//EventBuffer& eventBuffer = m_window.GetEventBuffer();
		//while (!eventBuffer.IsEmpty())
		//{
		//	std::optional<Event> event = eventBuffer.Pop();
		//	if (event->GetEventType() == EventType::WindowClose)
		//	{
		//		std::cout << "Close\n";
		//		m_window.Close();
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
		m_window.Close();
		m_running = false;
	}

	void Application::OnWindowClosed_Const(WindowClosedEvent& e) const
	{
		std::cout << "Close\n";
	}

	void Application::OnEvent(Event& e)
	{
		
	}
}