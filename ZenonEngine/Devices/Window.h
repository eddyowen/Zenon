#pragma once

#include "Core/Delegate.h"
#include "Core/Signal.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

		Signal<zn::WindowClosedEvent&> OnWindowsClosed;
		Signal<zn::WindowResizedEvent&> OnWindowResized;
		Signal<zn::KeyPressedEvent&> OnKeyPressed;

	protected:
		void CloseCallback();
		void WindowResizedCallback(int width, int height);
		void KeyPressedCallback(int key);

	private:
		bool ShouldClose() const;

		void Draw() const;
		void Clear() const;
		void SwapBuffers() const;
		void PollEvents() const;

		static void GLFW_CloseCallback(GLFWwindow* wnd);
		static void GLFW_FrameBufferResizeCallback(GLFWwindow* wnd, int width, int height);
		static void GLFW_KeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods);

#ifdef ZN_DEBUG
		static void APIENTRY OpenGLDebugOutput(GLenum source, GLenum type, unsigned int id,
			GLenum severity, GLsizei length, const char* message, const void* userParam);
#endif

	private:
		GLFWwindow* m_window = nullptr;
		const char* m_title = nullptr;

		int m_width = 0;
		int m_height = 0;

		// TEMPORAL ///////////////////////////////////////
		UniquePtr<zn::Shader> m_basicShader;
		UniquePtr<zn::VertexArray> m_vertexArray;

		UniquePtr<zn::Texture> m_texture;
		UniquePtr<zn::Texture> m_texture2;
		
		glm::mat4 m_transform;
		
		unsigned int VBO, EBO, VAO;

		float vertices[32] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};

		unsigned int indices[6] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};
		//////////////////////////////////////////////////
	};
}