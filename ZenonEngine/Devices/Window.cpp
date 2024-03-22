#include "Window.h"

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

		// TEMPORAL ///////////////////////////////////////
		m_BasicShader = CreateUnique<zn::Shader>("Basic Shader", "../../Resources/Shaders/vertex.glsl", "../../Resources/Shaders/fragment.glsl");

		//m_VertexArray = CreateUnique<zn::VertexArray>();
		//
		//VertexBuffer vertexBuffer{};
		//IndexBuffer indexBuffer{};
		//
		//m_VertexArray->Bind();
		//
		//vertexBuffer.SetData(vertices, sizeof(vertices));
		//indexBuffer.SetData(indices, 3);
		//
		//VertexBufferLayout vertexBufferlayout;
		//vertexBufferlayout.Push<float>(3);
		//
		//m_VertexArray->AddVertexBuffer(vertexBuffer, vertexBufferlayout);
		//
		//vertexBuffer.Unbind();
		//indexBuffer.Unbind();
		//
		//m_VertexArray->Unbind();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);
		//////////////////////////////////////////////////

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

			m_BasicShader->Bind();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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