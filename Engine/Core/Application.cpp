#include "Application.h"

#include "Log.h"
#include "Timer.h"

namespace zn
{
	bool Application::Init(const std::string& appName)
	{
		Log::Init();
		
		if (!m_window.Init(1200, 1200, appName))
		{
			ZN_CORE_CRITICAL("[Application::Init] Failed to initialize Window. Closing Application")
			return false;
		}

		if (!m_renderer.Init(m_window.GetWidth(), m_window.GetHeight()))
		{
			ZN_CORE_CRITICAL("[Application::Init] Failed to initialize Renderer. Closing Application")
			return false;
		}

		m_inputSystem.Init(m_window.GetNativeWindow());

		m_keyPressedConnection = EventSystem::Instance().Subscribe<KeyPressedEvent>(shared_from_this(), &Application::OnKeyPressed);
		m_scrollChangedConnection = EventSystem::Instance().Subscribe<ScrollChangedEvent>(shared_from_this(), &Application::OnScrollChanged);
		m_windowClosedConnection = EventSystem::Instance().Subscribe<WindowClosedEvent>(shared_from_this(), &Application::OnWindowClosed);
		m_windowResizedConnection = EventSystem::Instance().Subscribe<WindowResizedEvent>(shared_from_this(), &Application::OnWindowResized);
			
		return true;
	}

	void Application::Run()
	{
		const Time::Duration fixedDelta(1 / 60.f);
		double accumulator = 0.0;

		constexpr int maxUpdatesPerFrame = 5;
		constexpr double maxFrameTime = 0.25; // capped at 4 FPS equivalent
		
		Time::TimePoint previousTime = Time::GetCurrentTime();

		m_camera.SetPosition(glm::vec3(0, 0, -3.0f));
		
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
			
			m_renderer.ClearScreen(0.4f, 0.4f, 0.4f, 1.0f);
			m_renderer.Render(m_camera);
			
			m_window.RenderImGUI();
			m_window.SwapBuffers();
		}
	}

	bool Application::OnKeyPressed(const KeyPressedEvent& e)
	{
		ZN_CORE_TRACE("KeyPressedEvent: {}. ({}) repeats", static_cast<KeyCodeType>(e.KeyCode),  e.RepeatCount)
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
		
		m_renderer.SetAspectRatio(e.Width, e.Height);
		return true;
	}

	void Application::ProcessInput(float deltaTime)
	{
		if (m_inputSystem.GetKeyState(KeyCode::W))
			m_camera.ProcessKeyboard(Camera::MovementDirection::Forward, deltaTime);
		if (m_inputSystem.GetKeyState(KeyCode::S))
			m_camera.ProcessKeyboard(Camera::MovementDirection::Backward, deltaTime);
		if (m_inputSystem.GetKeyState(KeyCode::A))
			m_camera.ProcessKeyboard(Camera::MovementDirection::Left, deltaTime);
		if (m_inputSystem.GetKeyState(KeyCode::D))
			m_camera.ProcessKeyboard(Camera::MovementDirection::Right, deltaTime);
		
		m_camera.ProccessMoveMovement(m_inputSystem.GetMouseX(), m_inputSystem.GetMouseY());
	}
	
}
