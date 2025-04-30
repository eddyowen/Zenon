#include "Application.h"

#include <imgui_internal.h>

#include "Log.h"
#include "Timer.h"
#include "Resource/ResourceManager.h"

namespace zn
{
	bool Application::Init(const std::string& appName, uint32_t windowWidth, uint32_t windowHeight)
	{
		Log::Init();
		
		if (!m_window.Init(windowWidth, windowHeight, appName))
		{
			ZN_CORE_CRITICAL("[Application::Init] Failed to initialize Window. Closing Application")
			return false;
		}

		m_camera = { static_cast<float>(windowWidth) / static_cast<float>(windowHeight) };
		m_camera.SetPosition(glm::vec3(1.5f, 0.0f, 4.0f));
		// just so the camera doesn't jump with the first mouse interaction
		m_camera.SetLastMousePosition(windowWidth / 2.0f, windowHeight / 2.0f); 

		if (!m_renderer.Init(windowWidth, windowHeight))
		{
			ZN_CORE_CRITICAL("[Application::Init] Failed to initialize Renderer. Closing Application")
			return false;
		}

		m_inputSystem.Init(m_window.GetNativeWindow());

		m_keyPressedConnection = EventSystem::Instance().Subscribe<KeyPressedEvent>(shared_from_this(), &Application::OnKeyPressed);
		m_cursorMovedConnection = EventSystem::Instance().Subscribe<CursorMovedEvent>(shared_from_this(), &Application::OnCursosMoved);
		m_scrollChangedConnection = EventSystem::Instance().Subscribe<ScrollChangedEvent>(shared_from_this(), &Application::OnScrollChanged);
		m_windowClosedConnection = EventSystem::Instance().Subscribe<WindowClosedEvent>(shared_from_this(), &Application::OnWindowClosed);
		m_windowResizedConnection = EventSystem::Instance().Subscribe<WindowResizedEvent>(shared_from_this(), &Application::OnWindowResized);
			
		return true;
	}

	void Application::Run()
	{
		//const Time::Duration fixedDelta(1 / 60.f);
		//double accumulator = 0.0;
		//constexpr int maxUpdatesPerFrame = 5;
		//constexpr double maxFrameTime = 0.25; // capped at 4 FPS equivalent
		
		Time::TimePoint previousTime = Time::GetCurrentTime();
		
		while (!m_window.ShouldClose())
		{
			Time::TimePoint currentTime = Time::GetCurrentTime();
			Time::Duration frameDuration = currentTime - previousTime;
			double deltaTime = frameDuration.count();
			
			previousTime = currentTime;

			// Clamp frameTime to prevent big accumulator jumps on stalls
			//frameTime = std::min(frameTime, maxFrameTime);
			//accumulator += frameTime;
			//
			//int frameUpdates = 0;
			//while (accumulator >= fixedDelta.count())
			//{
			//	if (frameUpdates >= maxUpdatesPerFrame)
			//	{
			//		accumulator = 0.0;
			//		break;
			//	}
			//
			//	//Update Simulation(dt)
			//	accumulator -= fixedDelta.count();
			//	frameUpdates++;
			//}
			
			m_window.PollEvents();
			m_inputSystem.Update();
			
			ProcessInput(deltaTime);
			
			//double interpolationAlpha = accumulator / fixedDelta.count();
			
			m_renderer.Render(m_camera);
			
			m_window.RenderImGUI();
			m_window.SwapBuffers();
		}

		Shutdown();
	}

	void Application::Shutdown()
	{
		ResourceManager::Shutdown();
		m_renderer.Shutdown();
	}

	bool Application::OnKeyPressed(const KeyPressedEvent& e)
	{
		ZN_CORE_TRACE("KeyPressedEvent: {}. ({}) repeats", static_cast<KeyCodeType>(e.KeyCode),  e.RepeatCount)
		return true;
	}

	bool Application::OnCursosMoved(const CursorMovedEvent& e)
	{
		m_camera.ProccessMouseMovement(static_cast<float>(e.PosX), static_cast<float>(e.PosY));
		
		return true;
	}

	bool Application::OnScrollChanged(const ScrollChangedEvent& e)
	{
		m_camera.ProcessMouseScroll(e.OffsetX, e.OffsetY);
		return true;
	}

	bool Application::OnWindowClosed(const WindowClosedEvent& e)
	{
		ZN_CORE_TRACE("Window ClosedEvent"	)
		return true;
	}

	bool Application::OnWindowResized(const WindowResizedEvent& e)
	{
		ZN_CORE_TRACE("Window ResizedEvent")
		m_camera.SetViewportSize(e.Width, e.Height);
		return true;
	}

	void Application::ProcessInput(double deltaTime)
	{
		if (m_inputSystem.GetKeyState(KeyCode::W))
			m_camera.ProcessKeyboard(Camera::MovementDirection::Forward, deltaTime);
		if (m_inputSystem.GetKeyState(KeyCode::S))
			m_camera.ProcessKeyboard(Camera::MovementDirection::Backward, deltaTime);
		if (m_inputSystem.GetKeyState(KeyCode::A))
			m_camera.ProcessKeyboard(Camera::MovementDirection::Left, deltaTime);
		if (m_inputSystem.GetKeyState(KeyCode::D))
			m_camera.ProcessKeyboard(Camera::MovementDirection::Right, deltaTime);
	}
}
