# Muppet Graphics Rendering Library

Muppet is a C++ graphics rendering library that provides an easy-to-use API for 3D graphics. The library uses OpenGL, GLFW, and GLEW to enable efficient rendering of graphics on a variety of platforms.

## Features

Muppet provides the following features:

- Simple and intuitive API for creating and rendering 2D and 3D graphics.
- Efficient rendering using OpenGL and other third-party libraries.
- Lightweight and easy to use, with a minimal learning curve.

## Installation

To use Muppet in your project, simply clone the repository and include the necessary header files and libraries in your project. To build the library, you will need a C++ compiler and the necessary third-party libraries installed on your system.

Cross-platform compatibility is untested and it will likely require some work to be setup. Muppet is very much work in progress and should only be used as a quick try out.
Documentation still has to be written, Bugs still have to be fixed and many features are in progress.

## Usage

To use Muppet in your project, simply include the necessary header files and libraries and start using the API. Here's an example of how to create a simple 3D cube Using Muppet:

```c++
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
```


## Contributing

If you would like to contribute to Muppet, you can submit bug reports, feature requests, and pull requests on the GitHub repository. Before submitting a pull request, please ensure that your code follows the project's coding standards and that all tests pass successfully.

## License

Muppet is licensed under the MIT License. See the LICENSE file for more information.

## Credits

Muppet was created by [Your Name Here]. Thanks to the following open-source projects for their contributions to Muppet:

- [OpenGL](https://www.opengl.org/)
- [GLFW](https://www.glfw.org/)
- [GLEW](http://glew.sourceforge.net/)



