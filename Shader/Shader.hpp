#ifndef SHADER_HPP
#define SHADER_HPP

#include "glad/glad.h"

class Shader
{
	private:
		unsigned int	_id;
		char			*_filePath;
		GLenum			_shaderType;
		
		Shader(const Shader &) = delete;
		Shader &operator=(const Shader &) = delete;

	public:
		Shader();
		void	compileShader(const char *filePath, GLenum shaderType);
		~Shader();


		unsigned int getId() const;
};

#endif // SHADER_HPP