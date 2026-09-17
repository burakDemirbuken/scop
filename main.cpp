#include "scop/Camera.hpp"
#include "scop/Input.hpp"
#include "scop/Renderer.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace
{
	constexpr int SCR_WIDTH = 800;
	constexpr int SCR_HEIGHT = 600;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f);
	initializeInput(camera);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	try
	{
		Renderer renderer;

		double lastFpsUpdateTime = glfwGetTime();
		int frameCount = 0;
		float lastTime = 0.0f;
		
		while (!glfwWindowShouldClose(window))
		{
			float currentTime = glfwGetTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;
			processInput(deltaTime);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderer.render(currentTime, camera);

			++frameCount;
			double elapsed = currentTime - lastFpsUpdateTime;
			if (elapsed >= 0.25)
			{
				double fps = frameCount / elapsed;
				frameCount = 0;
				std::cout << "\rFPS: " << std::fixed << std::setprecision(1) << fps << "   " << std::flush;
				lastFpsUpdateTime = currentTime;
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	catch (const std::runtime_error &error)
	{
		std::cerr << error.what() << std::endl;
		glfwTerminate();
		return -1;
	}

	std::cout << std::endl;
	glfwTerminate();
	std::cout << "Program terminated successfully." << std::endl;
	return 0;
}
