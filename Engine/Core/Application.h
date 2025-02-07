#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Devices/Window.h"

namespace zn
{
	class Application : public EnableSharedFromThis<Application>
	{
	public:
		Application() = default;
		~Application() = default;
		
		Application(const Application& other) = delete;
		Application(Application&& other) noexcept = delete;
		
		Application& operator=(const Application& other) = delete;
		Application& operator=(Application&& other) noexcept = delete;
		
		bool Init();
		void Run();

		bool OnKeyPressed(const KeyPressedEvent& e);
		bool OnWindowClosed(const WindowClosedEvent& e);
		bool OnWindowResized(const WindowResizedEvent& e);

	private:
		bool m_isRunning = false;

		Window m_window{};

		EventConnection<KeyPressedEvent> m_keyPressedConnection;
		EventConnection<WindowClosedEvent> m_windowClosedConnection;
		EventConnection<WindowResizedEvent> m_windowResizedConnection;
	};
}