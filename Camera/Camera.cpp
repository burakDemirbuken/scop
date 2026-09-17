#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 direction, float speed, float sensitivity, float pitch, float yaw, float roll)
				: position(position), direction(glm::normalize(direction)), pitch(pitch), yaw(yaw), roll(roll), speed(speed), sensitivity(sensitivity)
{
	right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
}

Camera::~Camera()
{
}

glm::mat4	Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3	Camera::getPosition() const
{
	return position;
}

void	Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	if (direction == FORWARD)
		position += this->direction * speed * deltaTime;
	if (direction == BACKWARD)
		position -= this->direction * speed * deltaTime;
	if (direction == LEFT)
		position -= right * speed * deltaTime;
	if (direction == RIGHT)
		position += right * speed * deltaTime;
	if (direction == UP)
		position += glm::vec3(0.0f, 1.0f, 0.0f) * speed * deltaTime;
	if (direction == DOWN)
		position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed * deltaTime;
}

void	Camera::processMouseMovement(double xoffset, double yoffset, bool constrainPitch)
{
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void	Camera::updateCameraVectors()
{
	glm::vec3 newDirection;
	newDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newDirection.y = sin(glm::radians(pitch));
	newDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = glm::normalize(newDirection);

	right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
}