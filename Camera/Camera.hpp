#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
	private:

		glm::vec3 position;
		glm::vec3 direction;

		glm::vec3 right;

		float pitch;
		float yaw;
		float roll;

		float speed;
		float sensitivity;

		void	updateCameraVectors();

	public:

		Camera(glm::vec3 position, glm::vec3 direction, float speed = 1.0f, float sensitivity = 0.1f, float pitch = 0.0f, float yaw = -90.0f, float roll = 0.0f);
		~Camera();

		glm::mat4	getViewMatrix() const;
		glm::vec3	getPosition() const;

		void	processKeyboard(Camera_Movement direction, float deltaTime);
		void	processMouseMovement(double xoffset, double yoffset, bool constrainPitch = true);
};

#endif // CAMERA_HPP