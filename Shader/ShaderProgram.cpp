#include "ShaderProgram.hpp"
#include "glad/glad.h"
#include "Shader.hpp"
#include <stdexcept>

ShaderProgram::ShaderProgram()
{
}

void ShaderProgram::compile(const char *vertexPath, const char *fragmentPath)
{
	Shader vertexShader;
	Shader fragmentShader;

	try
	{
		vertexShader.compileShader(vertexPath, GL_VERTEX_SHADER);
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(std::string("Vertex shader compilation failed: ") + e.what());
	}
	try
	{
		fragmentShader.compileShader(fragmentPath, GL_FRAGMENT_SHADER);
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(std::string("Fragment shader compilation failed: ") + e.what());
	}
	

	_id = glCreateProgram();
	glAttachShader(_id, vertexShader.getId());
	glAttachShader(_id, fragmentShader.getId());
	glLinkProgram(_id);

	int success;
	char infoLog[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		throw std::runtime_error(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + infoLog);
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_id);
}

void ShaderProgram::use() const
{
	glUseProgram(_id);
}

unsigned int ShaderProgram::getId() const
{
	return _id;
}

void	ShaderProgram::setuniform(const std::string &name, bool value) const
{
	int location = glGetUniformLocation(_id, name.c_str());
	glUniform1i(location, value);
}

void	ShaderProgram::setuniform(const std::string &name, int value) const
{
	int location = glGetUniformLocation(_id, name.c_str());
	glUniform1i(location, value);
}

void	ShaderProgram::setuniform(const std::string &name, float value) const
{
	int location = glGetUniformLocation(_id, name.c_str());
	glUniform1f(location, value);
}

void	ShaderProgram::setuniform(const std::string &name, const glm::vec2 &value) const
{
	int location = glGetUniformLocation(_id, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void	ShaderProgram::setuniform(const std::string &name, const glm::vec3 &value) const
{
	int location = glGetUniformLocation(_id, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void	ShaderProgram::setuniform(const std::string &name, const glm::vec4 &value) const
{
	int location = glGetUniformLocation(_id, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void	ShaderProgram::setuniform(const std::string &name, const glm::mat2 &value) const
{
	int location = glGetUniformLocation(_id, name.c_str());
	glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
}

void	ShaderProgram::setuniform(const std::string &name, const glm::mat3 &value) const
{
	int location = glGetUniformLocation(_id, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void	ShaderProgram::setuniform(const std::string &name, const glm::mat4 &value) const
{
	int location = glGetUniformLocation(_id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);	
}