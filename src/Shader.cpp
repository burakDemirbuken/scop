#include "scop/Shader.hpp"
#include <fstream>
#include <sstream>

using namespace std;

Shader::Shader()
{
}

void Shader::compileShader(const char *filePath, GLenum shaderType)
{
	ifstream file(filePath);
	if (!file.is_open())
		throw runtime_error("Failed to open shader file");
	
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	std::string shaderSource = buffer.str();
	const char *shaderSourceCStr = shaderSource.c_str();

	this->_id = glCreateShader(shaderType);
	glShaderSource(this->_id, 1, &shaderSourceCStr, NULL);
	glCompileShader(this->_id);

	int success;
	char infoLog[512];
	glGetShaderiv(this->_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(_id, 512, NULL, infoLog);
		throw runtime_error(string("ERROR::SHADER::COMPILATION_FAILED\n") + infoLog);
	}
}



Shader::~Shader()
{
	glDeleteShader(_id);
}

unsigned int Shader::getId() const
{
	return _id;
}