#define STB_IMAGE_IMPLEMENTATION
#include "Shader/ShaderProgram.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera/Camera.hpp"

#include <iomanip>
#include <iostream>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void	inputController(float deltaTime);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool isSpacePressed = false;

Camera camera(glm::vec3(1.2f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f);

glm::vec2 lastMousePos(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

std::vector<int>	inputEvents;

std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			os << mat[j][i] << " ";
		}
		os << std::endl;
	}
	return os;
}


unsigned int	loadTexture(const char* path, GLenum s_wrap, GLenum t_wrap, GLint min_filter, GLint mag_filter)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(data);
	return texture;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
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
        return -1;
    }

	ShaderProgram shaderProgram;
	try
	{
		shaderProgram.compile("shader/vertex/vertexShader.vert", "shader/fragment/fragmentShader.frag");
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float vertices[] = {
		//vertex positions			// UV
		// front
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		// back
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		// right
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		// left
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		// top
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
		// bottom
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	};

	unsigned int floorIndices[] = {
		0, 1, 2, 1, 3, 2
	};

	unsigned int indices[] = {
		0, 1, 2, 1, 3, 2,
		4, 5, 6, 5, 7, 6,
		8, 9, 10, 9, 11, 10,
		12, 13, 14, 13, 15, 14,
		16, 17, 18, 17, 19, 18,
		20, 21, 22, 21, 23, 22
	};

	std::vector<glm::vec3> cubePos;

	for (int i = 0; i < 1000; i++)
	{
		cubePos.push_back(glm::vec3(
			static_cast<float>(rand() % 150 - 75) / 10.0f,
			static_cast<float>(rand() % 150 - 75) / 10.0f,
			static_cast<float>(rand() % 150 - 75) / 10.0f
		));
	}

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

	unsigned int texture0 = loadTexture("textures/container.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
	unsigned int texture1 = loadTexture("textures/face.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

	shaderProgram.use();
	shaderProgram.setuniform("texture0", 0);
	shaderProgram.setuniform("texture1", 1);

	double lastFpsUpdateTime = glfwGetTime();
    int frameCount = 0;

	float lastTime = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
		float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        inputController(deltaTime);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);

        shaderProgram.setuniform("view", camera.getViewMatrix());
		shaderProgram.setuniform("projection", projection);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		for (unsigned int i = 0; i < cubePos.size(); i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePos[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() + glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgram.setuniform("model", model);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		frameCount++;
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
    std::cout << std::endl;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(2, &texture0);
    glDeleteTextures(2, &texture1);
    glfwTerminate();
    std::cout << "Program terminated successfully." << std::endl;
	return 0;

}

void	inputController(float deltaTime)
{
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_ESCAPE) != inputEvents.end())
		glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_W) != inputEvents.end())
		camera.processKeyboard(FORWARD, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_S) != inputEvents.end())
		camera.processKeyboard(BACKWARD, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_A) != inputEvents.end())
		camera.processKeyboard(LEFT, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_D) != inputEvents.end())
		camera.processKeyboard(RIGHT, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_SPACE) != inputEvents.end())
		camera.processKeyboard(UP, deltaTime);
	if (std::find(inputEvents.begin(), inputEvents.end(), GLFW_KEY_LEFT_SHIFT) != inputEvents.end())
		camera.processKeyboard(DOWN, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		inputEvents.push_back(key);
	if (action == GLFW_RELEASE)
		inputEvents.erase(std::remove(inputEvents.begin(), inputEvents.end(), key), inputEvents.end());
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	glm::vec2 mousePos(xpos, -ypos);
	glm::vec2 mouseOffset = mousePos - lastMousePos;
	lastMousePos = mousePos;

	camera.processMouseMovement(mouseOffset.x, mouseOffset.y);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

