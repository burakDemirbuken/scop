#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Camera/Camera.hpp"
#include "Shader/ShaderProgram.hpp"
#include <glm/glm.hpp>

class Renderer
{
	public:
		Renderer();
		~Renderer();
		void render(float currentTime, const Camera &camera);

	private:
		struct Material
		{
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float shininess;
		};

		ShaderProgram spDefault;
		ShaderProgram spLight;
		Material material;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		glm::mat4 projection;
};

#endif