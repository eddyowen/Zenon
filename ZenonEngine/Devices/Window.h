#pragma once

#include "Core/Delegate.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Graphics/Shader.h"

struct GLFWwindow;

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
		Delegate<void(zn::WindowResizedEvent&)> WindowResizedDelegate;
		Delegate<void(zn::KeyPressedEvent&)> KeyPressedDelegate;

	protected:
		void CloseCallback();
		void WindowResizedCallback(int width, int height);
		void KeyPressedCallback(int key);

	private:
		bool ShouldClose() const;

		void Clear() const;
		void SwapBuffers() const;
		void PollEvents() const;

		static void GLFW_CloseCallback(GLFWwindow* wnd);
		static void GLFW_FrameBufferResizeCallback(GLFWwindow* wnd, int width, int height);
		static void GLFW_KeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods);

	private:
		GLFWwindow* m_Window = nullptr;
		const char* m_Title = nullptr;

		int m_Width = 0;
		int m_Height = 0;

		// TEMPORAL ///////////////////////////////////////
		UniquePtr<zn::Shader> m_BasicShader;
		
		unsigned int VBO, EBO, VAO;

		float vertices[12] = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};

		unsigned int indices[6] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};
		//////////////////////////////////////////////////
	};
}