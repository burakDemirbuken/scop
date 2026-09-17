#include "Input.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <vector>

namespace
{
	Camera *activeCamera = nullptr;
	std::vector<int> inputEvents;
	glm::vec2 lastMousePos(800.0f / 2.0f, 600.0f / 2.0f);
	bool firstMouse = true;
}

void initializeInput(Camera &camera)
{
	activeCamera = &camera;
}

void processInput(float deltaTime)
{
	if (activeCamera == nullptr)
		return;
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_ESCAPE) != inputEvents.end())
		glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_W) != inputEvents.end())
		activeCamera->processKeyboard(FORWARD, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_S) != inputEvents.end())
		activeCamera->processKeyboard(BACKWARD, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_A) != inputEvents.end())
		activeCamera->processKeyboard(LEFT, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_D) != inputEvents.end())
		activeCamera->processKeyboard(RIGHT, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_SPACE) != inputEvents.end())
		activeCamera->processKeyboard(UP, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_LEFT_SHIFT) != inputEvents.end())
		activeCamera->processKeyboard(DOWN, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_UP) != inputEvents.end())
		activeCamera->processMouseMovement(0.0, 0.5);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_DOWN) != inputEvents.end())
		activeCamera->processMouseMovement(0.0, -0.5);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_LEFT) != inputEvents.end())
		activeCamera->processMouseMovement(-0.5, 0.0);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_RIGHT) != inputEvents.end())
		activeCamera->processMouseMovement(0.5, 0.0);
}

void key_callback(GLFWwindow *, int key, int, int action, int)
{
	if (action == GLFW_PRESS)
		inputEvents.push_back(key);
	if (action == GLFW_RELEASE)
		inputEvents.erase(std::remove(inputEvents.begin(), inputEvents.end(), key), inputEvents.end());
}

void mouse_callback(GLFWwindow *, double xpos, double ypos)
{
	if (activeCamera == nullptr)
		return;
	glm::vec2 mousePos(xpos, -ypos);
	if (firstMouse)
	{
		lastMousePos = mousePos;
		firstMouse = false;
		return;
	}
	glm::vec2 mouseOffset = mousePos - lastMousePos;
	lastMousePos = mousePos;
	activeCamera->processMouseMovement(mouseOffset.x, mouseOffset.y);
}

void framebuffer_size_callback(GLFWwindow *, int width, int height)
{
	glViewport(0, 0, width, height);
}