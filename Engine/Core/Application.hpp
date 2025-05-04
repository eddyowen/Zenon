#pragma once

#include "Base.hpp"

#include "Camera/Camera.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Input/InputSystem.hpp"
#include "Platform/Window.hpp"
#include "Renderer/Renderer.hpp"

namespace zn
{
	class Application : public EnableSharedFromThis<Application>
	{
	public:
		Application() = default;
		~Application();
		
		Application(const Application& other) = delete;
		Application(Application&& other) noexcept = delete;
		
		Application& operator=(const Application& other) = delete;
		Application& operator=(Application&& other) noexcept = delete;
		
		b8 Init(const String& appName, u32 windowWidth, u32 windowHeight);
		void Run();
		void Shutdown();

		b8 OnKeyPressed(const KeyPressedEvent& e);
		b8 OnCursosMoved(const CursorMovedEvent& e);
		b8 OnScrollChanged(const ScrollChangedEvent& e);
		b8 OnWindowClosed(const WindowClosedEvent& e);
		b8 OnWindowResized(const WindowResizedEvent& e);

	private:
		void ProcessInput(f64 deltaTime);

	private:
		Window m_window{};
		InputSystem m_inputSystem{};
		Renderer m_renderer{};
		
		Camera m_camera;

		EventConnection<KeyPressedEvent> m_keyPressedConnection;
		EventConnection<CursorMovedEvent> m_cursorMovedConnection;
		EventConnection<ScrollChangedEvent> m_scrollChangedConnection;
		EventConnection<WindowClosedEvent> m_windowClosedConnection;
		EventConnection<WindowResizedEvent> m_windowResizedConnection;

		b8 m_initialized = false;
	};
}