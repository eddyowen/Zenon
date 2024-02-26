#pragma once

#include "Core/Base.h"
#include "Core/Delegate.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

#include <functional>

class GLFWwindow;

namespace zn
{
	class Window
	{
	public:
		Window() = default;
		~Window();

		[[nodiscard]]
		bool Init(int width, int height, const char* title);
		void Update();
		void Close();

		Delegate<void(zn::WindowClosedEvent&)> WindowClosedDelegate;
		Delegate<void(zn::KeyPressedEvent&)> KeyPressedDelegate;

	protected:
		void CloseCallback();
		void KeyPressedCallback(int key);

	private:
		bool ShouldClose() const;

		void PollEvents();
		void SwapBuffers();

		static void GLFW_CloseCallback(GLFWwindow* wnd);
		static void GLFW_KeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods);

	private:
		int m_width = 0;
		int m_height = 0;
		const char* m_title = nullptr;
		GLFWwindow* m_window = nullptr;
	};
}