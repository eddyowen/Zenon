#pragma once

#include "Core/Base.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace zn
{
	class Window
	{
	public:
		Window() = default;
		~Window();

		Window(const Window& other) = delete;
		Window(Window&& other) noexcept = delete;
		
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) noexcept = delete;

		[[nodiscard]]
		b8 Init(u32 width, u32 height, const String& name);
		
		void PollEvents() const;
		void RenderImGUI() const;
		void SwapBuffers() const;
		b8 ShouldClose() const;

		[[nodiscard]] 
		GLFWwindow* GetNativeWindow() const { return m_window; }

		u32 GetWidth() const { return m_width; }
		u32 GetHeight() const { return m_height; }

	protected:
		void CloseCallback();
		void WindowResizedCallback(int width, int height);
		void KeyPressedCallback(int key);

		static void GLFW_CloseCallback(GLFWwindow* wnd);
		static void GLFW_FrameBufferResizeCallback(GLFWwindow* wnd, int width, int height);
		static void GLFW_KeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods);
		static void GLFW_CursorPosCallback(GLFWwindow* window, f64 posX, f64 poxY);
		static void GLFW_ScrollCallback(GLFWwindow* window, f64 offsetX, f64 offsetY);

#ifdef ZN_DEBUG
		static void APIENTRY OpenGLDebugOutput(GLenum source, GLenum type, unsigned int id,
			GLenum severity, GLsizei length, const char* message, const void* userParam);
#endif

	private:
		GLFWwindow* m_window = nullptr;
		
		String m_name;
		
		u32 m_width = 0;
		u32 m_height = 0;
	};
}