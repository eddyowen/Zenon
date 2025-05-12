#include "Application.hpp"

#include "Log.hpp"
#include "Math/Math.hpp"
#include "Timer.hpp"

#include "Utils/Lifetime.hpp"

#include <imgui_internal.h>

#include "Assert.hpp"
#include "Resource/ResourceRegistry.hpp"

namespace zn
{
	Application::~Application()
	{
		
	}

	b8 Application::Init(const String& appName, u32 windowWidth, u32 windowHeight)
	{
		Log::Init();
		
		if (!m_window.Init(windowWidth, windowHeight, appName))
		{
			ZN_CORE_CRITICAL("[Application::Init] Failed to initialize Window. Closing Application");
			return false;
		}

		m_camera = { static_cast<f32>(windowWidth) / static_cast<f32>(windowHeight) };
		m_camera.SetPosition(math::v3(1.5f, 3.0f, 4.0f));
		// just so the camera doesn't jump with the first mouse interaction
		m_camera.SetLastMousePosition(windowWidth / 2.0f, windowHeight / 2.0f); 

		if (!m_renderer.Init(windowWidth, windowHeight))
		{
			ZN_CORE_CRITICAL("[Application::Init] Failed to initialize Renderer. Closing Application");
			return false;
		}

		m_inputSystem.Init(m_window.GetNativeWindow());

		m_keyPressedConnection = EventSystem::Instance().Subscribe<KeyPressedEvent>(shared_from_this(), &Application::OnKeyPressed);
		m_cursorMovedConnection = EventSystem::Instance().Subscribe<CursorMovedEvent>(shared_from_this(), &Application::OnCursosMoved);
		m_scrollChangedConnection = EventSystem::Instance().Subscribe<ScrollChangedEvent>(shared_from_this(), &Application::OnScrollChanged);
		m_windowClosedConnection = EventSystem::Instance().Subscribe<WindowClosedEvent>(shared_from_this(), &Application::OnWindowClosed);
		m_windowResizedConnection = EventSystem::Instance().Subscribe<WindowResizedEvent>(shared_from_this(), &Application::OnWindowResized);

		m_initialized = true;

		return true;
	}

	void Application::Run()
	{
		ZN_ASSERT(m_initialized, "Ran application before initializing it");
		
		if (!m_initialized)
			return;
		
		//const Time::Duration fixedDelta(1 / 60.f);
		//f64 accumulator = 0.0;
		//constexpr int maxUpdatesPerFrame = 5;
		//constexpr f64 maxFrameTime = 0.25; // capped at 4 FPS equivalent
		
		Time::TimePoint previousTime = Time::GetCurrentTime();
		
		while (!m_window.ShouldClose())
		{
			Time::TimePoint currentTime = Time::GetCurrentTime();
			Time::Duration frameDuration = currentTime - previousTime;
			f64 deltaTime = frameDuration.count();
			
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
			
			//f64 interpolationAlpha = accumulator / fixedDelta.count();
			
			m_renderer.Render(m_camera);
			
			m_window.RenderImGUI();
			m_window.SwapBuffers();
		}

		Shutdown();
	}

	void Application::Shutdown()
	{
		//ResourceManager::Shutdown();
		//m_renderer.Shutdown();
	}

	b8 Application::OnKeyPressed(const KeyPressedEvent& e)
	{
		ZN_CORE_TRACE("KeyPressedEvent: {}. ({}) repeats", static_cast<KeyCodeType>(e.KeyCode),  e.RepeatCount);
		return true;
	}

	b8 Application::OnCursosMoved(const CursorMovedEvent& e)
	{
		m_camera.ProccessMouseMovement(static_cast<f32>(e.PosX), static_cast<f32>(e.PosY));
		
		return true;
	}

	b8 Application::OnScrollChanged(const ScrollChangedEvent& e)
	{
		m_camera.ProcessMouseScroll(e.OffsetX, e.OffsetY);
		return true;
	}

	b8 Application::OnWindowClosed(const WindowClosedEvent& e)
	{
		ZN_CORE_TRACE("Window ClosedEvent");
		return true;
	}

	b8 Application::OnWindowResized(const WindowResizedEvent& e)
	{
		ZN_CORE_TRACE("Window ResizedEvent");
		m_camera.SetViewportSize(e.Width, e.Height);
		return true;
	}

	void Application::ProcessInput(f64 deltaTime)
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
