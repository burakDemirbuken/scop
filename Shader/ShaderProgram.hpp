
#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <string>
#include <glm/glm.hpp>

class ShaderProgram
{
	private:
		ShaderProgram(const ShaderProgram &) = delete;
		ShaderProgram &operator=(const ShaderProgram &) = delete;
		
		unsigned int _id;
		
	public:
		ShaderProgram();
		~ShaderProgram();

		void	compile(const char *vertexPath, const char *fragmentPath);
		void	use() const;
		unsigned int getId() const;

		void	setuniform(const std::string &name, bool value) const;
		void	setuniform(const std::string &name, int value) const;
		void	setuniform(const std::string &name, float value) const;

		void	setuniform(const std::string &name, const glm::vec2 &value) const;
		void	setuniform(const std::string &name, const glm::vec3 &value) const;
		void	setuniform(const std::string &name, const glm::vec4 &value) const;

		void	setuniform(const std::string &name, const glm::mat2 &value) const;
		void	setuniform(const std::string &name, const glm::mat3 &value) const;
		void	setuniform(const std::string &name, const glm::mat4 &value) const;
};

#endif // SHADERPROGRAM_HPP