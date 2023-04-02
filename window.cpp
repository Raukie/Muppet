#include "window.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <string>
#include "graphics.h"
#include <chrono>

#define _LOGGING

namespace Muppet
{
#pragma region Variable Initilization

	int Window::m_width = 0;
	int Window::m_height = 0;
	int Window::m_major = 0;
	int Window::m_minor = 0;
	int Window::m_swapInterval = 0;
	GLFWwindow* Window::m_window = nullptr;
	std::string Window::m_windowName = "DEBUG";
	float Window::m_delta = 0.0f;
	std::chrono::time_point<std::chrono::steady_clock> Window::m_lastUpdate;
#pragma endregion


	void Window::CreateWindow(const int p_width, const int p_height, 
	const int p_major, const int p_minor, std::string p_windowName,
	const int p_swapInterval = 0)
	{
		Window::m_width = p_width;
		Window::m_height = p_height;
		Window::m_major = p_major;
		Window::m_minor = p_minor;
		Window::m_windowName = p_windowName;
		Window::m_swapInterval = p_swapInterval;

		Window::InitGl();
	}


	//Main loop
	bool Window::IsOpen()
	{
		auto now = std::chrono::steady_clock::now();
		Window::m_delta = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastUpdate).count() / 1000000.0f;
		Window::m_lastUpdate = now;

		Input::m_mouseOldX = Input::m_mouseX;
		Input::m_mouseOldY = Input::m_mouseY;

		glfwGetCursorPos(Window::m_window, &Input::m_mouseX, &Input::m_mouseY);

		Graphics::Draw();
		glfwSwapBuffers(Window::m_window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		return !(glfwWindowShouldClose(Window::m_window));
	}

	void  Window::InitGl()
	{
		if (!glfwInit())
		{
			std::cout << "Error initialising glfw" << std::endl;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Window::m_major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Window::m_minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		#ifdef _LOGGING
				std::cout << "Creating window: " << std::endl;
		#endif // _LOGGING
		Window::m_window = glfwCreateWindow(Window::m_width, Window::m_height, 
											Window::m_windowName.c_str(), NULL, NULL);

		if (Window::m_window == NULL)
		{
			std::cout << "Error creating window" << std::endl;
			glfwTerminate();
		}

		#ifdef _LOGGING
			std::cout << "> Setting context" << std::endl;
		#endif // _LOGGING

		glfwMakeContextCurrent(Window::m_window);
		
		//Check for vsync
		if (m_swapInterval > 0)
		{
		glfwSwapInterval(m_swapInterval);
		#ifdef _LOGGING
			std::cout << "> Vsync enabled" << std::endl;
		#endif // _LOGGING
		} 
		#ifdef _LOGGING
		else
		{
			std::cout << "> Vsync disabled" << std::endl;
		}
		#endif // _LOGGING


		glewExperimental = GL_TRUE;
		#ifdef _LOGGING
				std::cout << "> Glew Experimental enabled" << std::endl;
		#endif // _LOGGING
		#ifdef _LOGGING
						std::cout << "> Initialising glew" << std::endl;
		#endif // _LOGGING
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error with glew" << std::endl;
		}
		#ifdef _LOGGING
				std::cout << "> Creating gl viewport" << std::endl;
		#endif // _LOGGING
		glViewport(0, 0, Window::m_width, Window::m_height);

		#ifdef _LOGGING
				std::cout << "> Creating ImGui Context" << std::endl;
		#endif // _LOGGING
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows




		ImGui::StyleColorsDark();


	
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		}

#ifdef _LOGGING
		std::cout << "> Setting ImGui Context to Opengl and GLFW3" << std::endl;
#endif // _LOGGING
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(Window::m_window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		Graphics::Init(Window::m_width, Window::m_height);
		glfwSetKeyCallback(Window::m_window, Input::KeyCallback);
		
		
		
}

}