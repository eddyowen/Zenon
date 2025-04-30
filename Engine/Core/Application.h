#pragma once

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
		
		bool Init(const std::string& appName, uint32_t windowWidth, uint32_t windowHeight);
		void Run();
		void Shutdown();

		bool OnKeyPressed(const KeyPressedEvent& e);
		bool OnCursosMoved(const CursorMovedEvent& e);
		bool OnScrollChanged(const ScrollChangedEvent& e);
		bool OnWindowClosed(const WindowClosedEvent& e);
		bool OnWindowResized(const WindowResizedEvent& e);

	private:
		void ProcessInput(double deltaTime);

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