#include "Renderer.hpp"

#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer()
	: VAO(0), VBO(0), EBO(0),
	  projection(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f))
{
	spDefault.compile("shader/vertex/vertexShader.vert", "shader/fragment/fragmentShader.frag");
	spLight.compile("shader/vertex/vertexShader.vert", "shader/fragment/light.frag");

	float vertices[] = {
		//vertex positions			// Normal
		// front
		 0.5f,  0.5f,  0.5f,		 0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,		 0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,		 0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,		 0.0f,  0.0f,  1.0f,
		// back
		-0.5f,  0.5f, -0.5f,		 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,		 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,		 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,		 0.0f,  0.0f, -1.0f,
		// right
		0.5f,  0.5f, -0.5f,			 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,			 1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,			 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,			 1.0f,  0.0f,  0.0f,
		// left
		-0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,
		// top
		 0.5f,  0.5f,  0.5f,		 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,		 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,		 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,		 0.0f,  1.0f,  0.0f,
		// bottom
		 0.5f, -0.5f,  0.5f,		 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,		 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,		 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,		 0.0f, -1.0f,  0.0f,
	};

	const unsigned int indices[] = {
		0, 1, 2,
		1, 3, 2,
		4, 5, 6,
		5, 7, 6,
		8, 9, 10,
		9, 11, 10,
		12, 13, 14,
		13, 15, 14,
		16, 17, 18,
		17, 19, 18,
		20, 21, 22,
		21, 23, 22
	};
	material = {
		{0.0215f, 0.1745f, 0.0215f},
		{0.07568f, 0.61424f, 0.07568f},
		{0.633f, 0.727811f, 0.633f},
		0.6f * 128
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Renderer::render(float currentTime, const Camera &camera)
{
	glm::vec3 lightPosition(sin(currentTime) * 2.0f, cos(currentTime) * 2.0f, sin(currentTime * 0.5f) * 2.0f);
	glm::vec3 lightColor((sin(currentTime) + 1.0f) / 2.0f, (cos(currentTime) + 1.0f) / 2.0f,
		(sin(currentTime * 0.5f) + 1.0f) / 2.0f);
	glBindVertexArray(VAO);
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(0.5f, 1.0f, 0.0f));
	spLight.use();
	spLight.setuniform("view", camera.getViewMatrix());
	spLight.setuniform("projection", projection);
	spLight.setuniform("model", model);
	spLight.setuniform("viewPos", camera.getPosition());
	spLight.setuniform("light.position", lightPosition);
	spLight.setuniform("light.ambient", lightColor * 0.1f);
	spLight.setuniform("light.diffuse", lightColor * 0.8f);
	spLight.setuniform("light.specular", lightColor);
	spLight.setuniform("material.ambient", material.ambient);
	spLight.setuniform("material.diffuse", material.diffuse);
	spLight.setuniform("material.specular", material.specular);
	spLight.setuniform("material.shininess", material.shininess);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	const glm::vec3 wallPositions[] = {{0.0f, -5.0f, 0.0f}, {0.0f, 5.0f, 0.0f}, {-5.0f, 0.0f, 0.0f},
		{5.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -5.0f}};
	const glm::vec3 wallScales[] = {{10.0f, 0.1f, 10.0f}, {10.0f, 0.1f, 10.0f}, {0.1f, 10.0f, 10.0f},
		{0.1f, 10.0f, 10.0f}, {10.0f, 10.0f, 0.1f}};
	for (int i = 0; i < 5; ++i)
	{
		model = glm::scale(glm::translate(glm::mat4(1.0f), wallPositions[i]), wallScales[i]);
		spLight.setuniform("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	spDefault.use();
	model = glm::scale(glm::translate(glm::mat4(1.0f), lightPosition), glm::vec3(0.1f));
	spDefault.setuniform("view", camera.getViewMatrix());
	spDefault.setuniform("projection", projection);
	spDefault.setuniform("model", model);
	spDefault.setuniform("color", lightColor);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}