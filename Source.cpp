#include "muppet.h" 
#include "GLFW/glfw3.h"


int main()
{
	Muppet::Window::CreateWindow(1000, 1000, 3, 3, "Debug", 1);
	Muppet::Graphics::m_camera->SetPosition(glm::vec3(-1, 1, 0));
	Muppet::Graphics::m_camera->LookAt(glm::vec3(1.0f, 1.0f, 1.0f));
	Muppet::Graphics::LoadObject("cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	while (Muppet::Window::IsOpen())
	{
		if (Muppet::Input::IsPressed(GLFW_KEY_W))
		{
			Muppet::Graphics::m_camera->processKeyboard(0, Muppet::Window::m_delta);
		}

		if (Muppet::Input::IsPressed(GLFW_KEY_A))
		{
			Muppet::Graphics::m_camera->processKeyboard(2, Muppet::Window::m_delta);
		}

		if (Muppet::Input::IsPressed(GLFW_KEY_S))
		{
			Muppet::Graphics::m_camera->processKeyboard(1, Muppet::Window::m_delta);
		}

		if (Muppet::Input::IsPressed(GLFW_KEY_D))
		{
			Muppet::Graphics::m_camera->processKeyboard(3, Muppet::Window::m_delta);

		}
	}
	
	return 0;
}