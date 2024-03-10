#include "Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

#include <iostream>

namespace zn
{
	Window::~Window()
	{
		glfwTerminate();
	}

	bool Window::Init(int width, int height, const char* title)
	{
		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW" << std::endl;
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_Window)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			glfwTerminate();
			return false;
		}

		// Setup user pointer as this is the only way 
		// I have to access data within GLFW callbacks
		m_Width = width;
		m_Height = height;
		m_Title = title;
		glfwSetWindowUserPointer(m_Window, this);

		// Setup Event Callbacks here
		glfwSetWindowCloseCallback(m_Window, &Window::GLFW_CloseCallback);
		glfwSetFramebufferSizeCallback(m_Window, &Window::GLFW_FrameBufferResizeCallback);
		glfwSetKeyCallback(m_Window, &Window::GLFW_KeyCallback);

		return true;
	}

	void Window::CloseCallback()
	{
		WindowClosedEvent e;
		WindowClosedDelegate(e);
	}

	void Window::WindowResizedCallback(int width, int height)
	{
		m_Width = width;
		m_Height = height;

		glViewport(0, 0, m_Width, m_Height);

		WindowResizedEvent e{m_Width, m_Height};
		WindowResizedDelegate(e);
	}
	
	void Window::KeyPressedCallback(int key)
	{
		KeyPressedEvent e(key, 0);
		KeyPressedDelegate(e);
	}

	void Window::GLFW_CloseCallback(GLFWwindow* wnd)
	{ 
		((Window*)glfwGetWindowUserPointer(wnd))->CloseCallback();
	}

	void Window::GLFW_FrameBufferResizeCallback(GLFWwindow* wnd, int width, int height)
	{
		((Window*)glfwGetWindowUserPointer(wnd))->WindowResizedCallback(width, height);
	}
	
	void Window::GLFW_KeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods)
	{
		Window* window = ((Window*)glfwGetWindowUserPointer(wnd));

		switch (action)
		{
			case GLFW_PRESS:
			{
				window->KeyPressedCallback(key);
				break;
			}
			case GLFW_RELEASE:
			{
				//KeyReleasedEvent event;
				break;
			}
			case GLFW_REPEAT:
			{
				//KeyPressedEvent event(key, 1);
				break;
			}
		}
	}

	void Window::Update()
	{
		if (!ShouldClose())
		{
			Clear();
			SwapBuffers();
			PollEvents();
		}
	}

	void Window::Close()
	{
		glfwSetWindowShouldClose(m_Window, GL_TRUE);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::Clear() const
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(m_Window);
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
	}
}