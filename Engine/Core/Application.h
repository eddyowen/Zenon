#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Devices/Window.h"

namespace zn
{
	class Application
	{
	public:
		Application() = default;
		~Application() = default;
		
		bool Init();
		void Run();
		
		void OnKeyPressed(KeyPressedEvent& e);
		void OnWindowClosed(WindowClosedEvent& e);
		void OnWindowResized(WindowResizedEvent& e);

	private:
		bool m_isRunning = false;
		Window m_window{};
	};
}