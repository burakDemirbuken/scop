
#include "Shader/ShaderProgram.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iomanip>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

	float vertices[] = {
		//vertex positions		//color
		 0.5f,  0.5f, 0.0f,		1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,		0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,		0.0f,  0.0f, 1.0f
	};

	float vertices2[] = {
		//vertex positions		//color
		 0.5f,  0.5f, 0.0f,		1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,		0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		0.0f,  0.0f, 1.0f
	};

    unsigned int VBO[2], VAO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
	
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
    
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0); 

    double lastFpsUpdateTime = glfwGetTime();
    int frameCount = 0;


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();

		int zamanAdresi = glGetUniformLocation(shaderProgram.getId(), "oyunZamani");

		shaderProgram.setuniform("intensity", float(sin(glfwGetTime()) * 0.5 + 0.5));
		
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

        frameCount++;
        double currentTime = glfwGetTime();
        double elapsed = currentTime - lastFpsUpdateTime;
        if (elapsed >= 0.25)
        {
            double fps = frameCount / elapsed;
            std::cout << "\rFPS: " << std::fixed << std::setprecision(1) << fps << "   " << std::flush;
            frameCount = 0;
            lastFpsUpdateTime = currentTime;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << std::endl;

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

