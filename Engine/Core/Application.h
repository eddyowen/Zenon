#pragma once

#include "Base.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Camera/Camera.h"
#include "Input/InputSystem.h"
#include "Platform/Window.h"
#include "Renderer/Renderer.h"

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
	};
}