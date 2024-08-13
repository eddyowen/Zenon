#include "Window.h"

#include "Core/Log.h"
#include "Core/FileSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
			ZN_CORE_CRITICAL("Failed to initialize GLFW");
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef ZN_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_Window)
		{
			ZN_CORE_CRITICAL("Failed to create GLFW window");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			ZN_CORE_CRITICAL("Failed to initialize GLAD");
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

#ifdef ZN_DEBUG
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			
			ZN_CORE_INFO("OpenGL Debug Context succesfully initialized");
		}
#endif
		// TEMPORAL ///////////////////////////////////////
		m_BasicShader = CreateUnique<zn::Shader>("Basic Shader", 
			FileSystem::GetPath("Content/Shaders/vertex.glsl").c_str(), FileSystem::GetPath("Content/Shaders/fragment.glsl").c_str());

		m_Texture = CreateUnique<zn::Texture>(FileSystem::GetPath("Content/Textures/wall.jpg"));
		m_Texture2 = CreateUnique<zn::Texture>(FileSystem::GetPath("Content/Textures/george.jpg"));

		m_VertexArray = CreateUnique<zn::VertexArray>();
		m_VertexArray->Bind();

		VertexBuffer vertexBuffer{};
		vertexBuffer.Bind();
		vertexBuffer.SetData(vertices, sizeof(vertices));
		
		VertexBufferLayout vertexBufferLayout;
		vertexBufferLayout.PushElement<float>(3);
		vertexBufferLayout.PushElement<float>(3);
		vertexBufferLayout.PushElement<float>(2); //texture coords

		m_VertexArray->AddVertexBuffer(vertexBuffer, vertexBufferLayout);

		IndexBuffer indexBuffer{};
		indexBuffer.Bind();
		indexBuffer.SetData(indices, 6);
		
		vertexBuffer.Unbind();

		//// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		//indexBuffer.Unbind();
		
		m_VertexArray->Unbind();
		// TEMPORAL ///////////////////////////////////////
		m_Transform = glm::mat4(1.0f);
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
			Draw();
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

	void Window::Draw() const
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, glm::radians(float(glfwGetTime() * 120.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

		m_Texture->Bind();
		m_BasicShader->Bind();
		m_BasicShader->SetInt("texture1", 0);
		m_BasicShader->SetInt("texture2", 1);

		m_Texture->Bind();
		m_Texture2->Bind(1);
		m_VertexArray->Bind();

		m_BasicShader->SetMat4("transform", transform);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

#ifdef ZN_DEBUG
	void APIENTRY Window::OpenGLDebugOutput(GLenum source, GLenum type, unsigned int id,
		GLenum severity, GLsizei length, const char* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) 
			return;

		std::string messageSource;
		switch (source)
		{
			case GL_DEBUG_SOURCE_API:             messageSource = "Source: API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   messageSource = "Source: Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: messageSource = "Source: Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     messageSource = "Source: Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     messageSource = "Source: Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           messageSource = "Source: Other"; break;
		} 

		std::string messageType;
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:               messageType = "Type: Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: messageType = "Type: Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  messageType = "Type: Undefined Behaviour"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         messageType = "Type: Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         messageType = "Type: Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              messageType = "Type: Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          messageType = "Type: Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           messageType = "Type: Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               messageType = "Type: Other"; break;
		}

		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				ZN_CORE_ERROR(std::format("Debug message ({}): {}\n{}\n{}\n{}\n", id, message, messageSource, messageType, "Severity: high"));
				break;

			case GL_DEBUG_SEVERITY_MEDIUM:  
				ZN_CORE_WARN(std::format("Debug message ({}): {}\n{}\n{}\n{}\n", id, message, messageSource, messageType, "Severity: medium"));
				break;

			case GL_DEBUG_SEVERITY_LOW:          
				ZN_CORE_TRACE(std::format("Debug message ({}): {}\n{}\n{}\n{}\n", id, message, messageSource, messageType, "Severity: low"));
				break;

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				ZN_CORE_TRACE(std::format("Debug message ({}): {}\n{}\n{}\n{}\n", id, message, messageSource, messageType, "Severity: notification"));
				break;
		}

		ZN_CORE_ASSERT(false);
	}
#endif
}