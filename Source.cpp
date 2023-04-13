#include "muppet.h" 
#include "GLFW/glfw3.h"
#include "Minecraft_Demo.h"

#define WINDOW_SIZE 2.0f

#ifdef _WIN32
#include <windows.h>
extern "C" _declspec(dllexport) DWORD NvOptimusEnablement = true;
#endif // __WIN32

void CubeScene(int w, int h, int z, float gap)
{
	glEnable(GL_CULL_FACE);
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
	glCullFace(GL_BACK);
}

void DrawMethodMenu()
{
	ImGui::Begin("Draw methods");
	ImGui::SetWindowFontScale(WINDOW_SIZE);
	if (ImGui::Button("GL_TRIANGLES"))
		Muppet::Graphics::SetGlobalDrawMethod(GL_TRIANGLES);
	if (ImGui::Button("GL_POINTS"))
		Muppet::Graphics::SetGlobalDrawMethod(GL_POINTS);
	if (ImGui::Button("GL_LINES"))
		Muppet::Graphics::SetGlobalDrawMethod(GL_LINES);
	if (ImGui::Button("GL_LINE_STRIP"))
		Muppet::Graphics::SetGlobalDrawMethod(GL_LINE_STRIP);
	if (ImGui::Button("GL_TRIANGLE_STRIP"))
		Muppet::Graphics::SetGlobalDrawMethod(GL_TRIANGLE_STRIP);
	ImGui::End();
}

void CityScene()
{
	std::shared_ptr<Muppet::Object> Cube = Muppet::Graphics::LoadObject("city.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)).lock();
	Cube->m_drawMethod = GL_TRIANGLES;
}

void TransformMenu(std::shared_ptr<Muppet::Object> obj)
{
	ImGui::Begin("Properties");
	ImGui::SetWindowFontScale(WINDOW_SIZE);
	ImGui::Text("Transform");
	ImGui::SliderFloat("tX", &obj->m_transform.m_position.x, -10.0f, 10.0f);
	ImGui::SliderFloat("tY", &obj->m_transform.m_position.y, -10.0f, 10.0f);
	ImGui::SliderFloat("tZ", &obj->m_transform.m_position.z, -10.0f, 10.0f);
	ImGui::Text("Rotation");
	ImGui::SliderFloat("rX", &obj->m_transform.m_rotation.x, 0, 360.0f);
	ImGui::SliderFloat("rY", &obj->m_transform.m_rotation.y, 0, 360.0f);
	ImGui::SliderFloat("rZ", &obj->m_transform.m_rotation.z, 0, 360.0f);
	ImGui::Text("Scaling");
	ImGui::SliderFloat("sX", &obj->m_transform.m_scale.x, -10.0f, 10.0f);
	ImGui::SliderFloat("sY", &obj->m_transform.m_scale.y, -10.0f, 10.0f);
	ImGui::SliderFloat("sZ", &obj->m_transform.m_scale.z, -10.0f, 10.0f);
	ImGui::End();
	obj->m_transform.UpdateMatrix();
	const float* pS = (const float*)glm::value_ptr(obj->m_transform.m_matrix);
	ImGui::Begin("Matrix");
	ImGui::SetWindowFontScale(WINDOW_SIZE);
	ImGui::Text("%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n", pS[0], pS[1], pS[2], pS[3], pS[4], pS[5], pS[6], pS[7], pS[8], pS[9], pS[10], pS[11], pS[12], pS[13], pS[14], pS[15]);
	ImGui::End();

}


int main()
{

	Muppet::Window::CreateMuppetWindow(1920, 1080, 3, 3, "Debug", 0);
	Muppet::Graphics::m_camera->SetPosition(glm::vec3(-1, 1, 0));
	Muppet::Graphics::m_camera->LookAt(glm::vec3(1.0f, 1.0f, 1.0f));
	
	Muppet::Graphics::m_camera->SetPosition(glm::vec3(0, WORLD_HEIGHT, 0));
	Game game;
	
	
	//std::shared_ptr<Muppet::Object> cat = Muppet::Graphics::LoadObject("cat.obj").lock();
	//game.GenWorld(10);
	CubeScene(90, 90, 90, 10.0f);



	ImGuiIO& io = ImGui::GetIO(); (void)io;
	float multi = 1;
	while (Muppet::Window::IsOpen())
	{
		game.ManageChunks(10, Muppet::Graphics::m_camera->position);
		
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

		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();

		ImGui::NewFrame();

	
		ImGui::Begin("Debug");
		ImGui::SetWindowFontScale(WINDOW_SIZE);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();

		DrawMethodMenu();
		//TransformMenu(cat);

		ImGui::Begin("Camera");
		ImGui::SetWindowFontScale(WINDOW_SIZE);
		ImGui::SliderFloat("SpeedMultiplier", &multi, 0.0f, 20.0f);
		ImGui::End();
		Muppet::Graphics::m_camera->movementSpeed = 2.5f * multi;
		ImGui::EndFrame();
		ImGui::Render();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	
	return 0;
}