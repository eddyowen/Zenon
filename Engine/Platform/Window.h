#pragma once

#include "Renderer/VertexArray.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace zn
{
	class Window
	{
	public:
		Window() = default;
		~Window();

		[[nodiscard]]
		bool Init(uint32_t width, uint32_t height, const std::string& name);
		
		void PollEvents() const;
		void RenderImGUI() const;
		void SwapBuffers() const;
		bool ShouldClose() const;

		[[nodiscard]] 
		GLFWwindow* GetNativeWindow() const { return m_window; }

		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }

	protected:
		void CloseCallback();
		void WindowResizedCallback(int width, int height);
		void KeyPressedCallback(int key);

		static void GLFW_CloseCallback(GLFWwindow* wnd);
		static void GLFW_FrameBufferResizeCallback(GLFWwindow* wnd, int width, int height);
		static void GLFW_KeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods);
		static void GLFW_CursorPosCallback(GLFWwindow* window, double posX, double poxY);
		static void GLFW_ScrollCallback(GLFWwindow* window, double offsetX, double offsetY);

#ifdef ZN_DEBUG
		static void APIENTRY OpenGLDebugOutput(GLenum source, GLenum type, unsigned int id,
			GLenum severity, GLsizei length, const char* message, const void* userParam);
#endif

	private:
		GLFWwindow* m_window = nullptr;
		
		std::string m_name;
		
		uint32_t m_width = 0;
		uint32_t m_height = 0;
	};
}