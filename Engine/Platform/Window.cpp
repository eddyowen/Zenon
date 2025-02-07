#include "Window.h"

#include "Core/Log.h"
#include "Core/Assert.h"
#include "FileSystem/FileSystem.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace zn
{
	Window::~Window()
	{
		m_window = nullptr;
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

		m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_window)
		{
			ZN_CORE_CRITICAL("Failed to create GLFW window")
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1); // Force VSYNC

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			ZN_CORE_CRITICAL("Failed to initialize GLAD")
			glfwTerminate();
			return false;
		}
		
		const char* glslVersion = "#version 450";
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init(glslVersion);
		
		// Setup user pointer as this is the only way 
		// I have to access data within GLFW callbacks
		m_width = width;
		m_height = height;
		m_title = title;
		glfwSetWindowUserPointer(m_window, this);

		// Setup Event Callbacks here
		glfwSetWindowCloseCallback(m_window, &Window::GLFW_CloseCallback);
		glfwSetFramebufferSizeCallback(m_window, &Window::GLFW_FrameBufferResizeCallback);
		glfwSetKeyCallback(m_window, &Window::GLFW_KeyCallback);

#ifdef ZN_DEBUG
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			
			ZN_CORE_INFO("OpenGL Debug Context succesfully initialized")
		}
#endif
		
		// TEMPORAL ///////////////////////////////////////
		m_basicShader = CreateUnique<zn::Shader>("Basic Shader", "Content/Shaders/vertex.glsl", "Content/Shaders/fragment.glsl");

		m_texture = CreateUnique<zn::Texture>("Content/Textures/wall.jpg");
		m_texture2 = CreateUnique<zn::Texture>("Content/Textures/george.jpg");

		m_vertexArray = CreateUnique<zn::VertexArray>();
		m_vertexArray->Bind();

		VertexBuffer vertexBuffer{};
		vertexBuffer.Bind();
		vertexBuffer.SetData(vertices, sizeof(vertices));
		
		VertexBufferLayout vertexBufferLayout;
		vertexBufferLayout.PushElement<float>(3);
		vertexBufferLayout.PushElement<float>(3);
		vertexBufferLayout.PushElement<float>(2); //texture coords

		m_vertexArray->AddVertexBuffer(vertexBuffer, vertexBufferLayout);

		IndexBuffer indexBuffer{};
		indexBuffer.Bind();
		indexBuffer.SetData(indices, 6);
		
		vertexBuffer.Unbind();

		//// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		//indexBuffer.Unbind();
		
		m_vertexArray->Unbind();
		// TEMPORAL ///////////////////////////////////////
		m_transform = glm::mat4(1.0f);
		return true;
	}

	void Window::CloseCallback()
	{
		glfwSetWindowShouldClose(m_window, GL_TRUE);
		
		WindowClosedEvent e;
		EventSystem::Instance().Post(e);
	}

	void Window::WindowResizedCallback(int width, int height)
	{
		m_width = width;
		m_height = height;

		glViewport(0, 0, m_width, m_height);

		WindowResizedEvent e{m_width, m_height};
		EventSystem::Instance().Post(e);
	}
	
	void Window::KeyPressedCallback(int key)
	{
		KeyPressedEvent e(key, 0);
		EventSystem::Instance().Post(e);
	}

	void Window::GLFW_CloseCallback(GLFWwindow* wnd)
	{ 
		static_cast<Window*>(glfwGetWindowUserPointer(wnd))->CloseCallback();
	}

	void Window::GLFW_FrameBufferResizeCallback(GLFWwindow* wnd, int width, int height)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(wnd))->WindowResizedCallback(width, height);
	}
	
	void Window::GLFW_KeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods)
	{
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(wnd));

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

	void Window::Cleanup()
	{
		// ImGui Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		
		glfwDestroyWindow(m_window);
		glfwTerminate();

		m_window = nullptr;
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}

	void Window::Draw() const
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, glm::radians(float(glfwGetTime() * 120.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

		m_texture->Bind();
		m_basicShader->Bind();
		m_basicShader->SetInt("texture1", 0);
		m_basicShader->SetInt("texture2", 1);

		m_texture->Bind();
		m_texture2->Bind(1);
		m_vertexArray->Bind();

		m_basicShader->SetMat4("transform", transform);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Window::Clear() const
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(m_window);
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
		
		if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
		}
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
				ZN_CORE_ERROR("Debug message ({}): {}\n{}\n{}\n{}\n", id, message, messageSource, messageType, "Severity: high")
				break;

			case GL_DEBUG_SEVERITY_MEDIUM:  
				ZN_CORE_WARN("Debug message ({}): {}\n{}\n{}\n{}\n", id, message, messageSource, messageType, "Severity: medium")
				break;

			case GL_DEBUG_SEVERITY_LOW:          
				ZN_CORE_TRACE("Debug message ({}): {}\n{}\n{}\n{}\n", id, message, messageSource, messageType, "Severity: low")
				break;

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				ZN_CORE_TRACE("Debug message ({}): {}\n{}\n{}\n{}\n", id, message, messageSource, messageType, "Severity: notification")
				break;
		}

		ZN_CORE_ASSERT(false)
	}
#endif
}
