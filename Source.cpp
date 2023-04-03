#include "muppet.h" 
#include "GLFW/glfw3.h"

void CubeScene(int w, int h, int z, float gap)
{
	std::shared_ptr<Muppet::Object> Cube = Muppet::Graphics::LoadObject("Cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)).lock();
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			for (int p = 0; p < z; p++)
			{
				Cube->Clone(glm::vec3((1.0f + gap)  * i, (1.0f + gap) * j, (1.0f + gap) * p), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
	}
	Cube->m_drawMethod = GL_TRIANGLES;
	Cube->GenInstanceBuffer();
}

void CityScene()
{
	std::shared_ptr<Muppet::Object> Cube = Muppet::Graphics::LoadObject("city.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)).lock();
	Cube->m_drawMethod = GL_TRIANGLES;
}



int main()
{

	Muppet::Window::CreateMuppetWindow(1000, 1000, 3, 3, "Debug", 0);
	Muppet::Graphics::m_camera->SetPosition(glm::vec3(-1, 1, 0));
	Muppet::Graphics::m_camera->LookAt(glm::vec3(1.0f, 1.0f, 1.0f));
	
	CubeScene(100, 100, 100, 1.0f);
	
	//std::weak_ptr<Muppet::Object> weakCat = Muppet::Graphics::LoadObject("cat.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(0.03f, 0.03f, 0.03f));
	//Muppet::Graphics::CopyObject(weakCat, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.03f, 0.03f, 0.03f));

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	float multi = 1;
	while (Muppet::Window::IsOpen())
	{
		Muppet::Graphics::Draw();

		if (Muppet::Input::IsPressed(GLFW_KEY_LEFT_SHIFT))
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

			Muppet::Graphics::m_camera->processMouseMovement(Muppet::Input::m_mouseX - Muppet::Input::m_mouseOldX, Muppet::Input::m_mouseOldY - Muppet::Input::m_mouseY);
		}

		//std::shared_ptr<Muppet::Object> cat = weakCube.lock();

		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();

		ImGui::NewFrame();

		ImGui::Begin("Debug");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();

		
		ImGui::Begin("Camera");
		ImGui::SliderFloat("SpeedMultiplier", &multi, 0.0f, 20.0f);
		ImGui::End();
		Muppet::Graphics::m_camera->movementSpeed = 2.5f * multi;
		/*
		ImGui::Begin("Properties");
		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("Transform");
		ImGui::SliderFloat("tX", &cat->m_transform.m_position.x, -10.0f, 10.0f);
		ImGui::SliderFloat("tY", &cat->m_transform.m_position.y, -10.0f, 10.0f);
		ImGui::SliderFloat("tZ", &cat->m_transform.m_position.z, -10.0f, 10.0f);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("rX", &cat->m_transform.m_rotation.x, 0, 360.0f);
		ImGui::SliderFloat("rY", &cat->m_transform.m_rotation.y, 0, 360.0f);
		ImGui::SliderFloat("rZ", &cat->m_transform.m_rotation.z, 0, 360.0f);
		ImGui::Text("Scaling");
		ImGui::SliderFloat("sX", &cat->m_transform.m_scale.x, -10.0f, 10.0f);
		ImGui::SliderFloat("sY", &cat->m_transform.m_scale.y, -10.0f, 10.0f);
		ImGui::SliderFloat("sZ", &cat->m_transform.m_scale.z, -10.0f, 10.0f);
		ImGui::End();

		cat->UpdateMatrix();
		*/
		ImGui::EndFrame();
		ImGui::Render();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		
	}

	
	return 0;
}