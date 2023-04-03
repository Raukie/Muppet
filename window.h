#pragma once
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


namespace Muppet
{
	//Window class for creating a window using glfw3
	class Window
	{
	public:
		static float m_delta;

#pragma region Variables
		static GLFWwindow* m_window;
#pragma endregion

		//Function for the window
		static void CreateMuppetWindow(const int p_width, const int p_height,
			const int p_major, const int p_minor, std::string p_windowName, 
			const int p_swapInterval
		);

		static bool IsOpen();

	private:
#pragma region Private Variables
		static std::chrono::steady_clock::time_point m_lastUpdate;
		static int m_width;
		static int m_height;
		static int m_major;
		static int m_minor;
		static int m_swapInterval;
		static std::string m_windowName;
#pragma endregion
		

		static void InitGl();

	};
}