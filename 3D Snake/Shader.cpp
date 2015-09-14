#include "Shader.h"

Shader::Shader(const GLchar* const vertexPath, const GLchar* const fragmentPath)
{
	// Get vertex/fragment source from file
	std::string vertexCode = getShaderText(vertexPath);
	std::string fragmentCode = getShaderText(fragmentPath);

	// Create vertex/fragment shader
	GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexCode.c_str());
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

	// Create shader program
	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	// Print linking errors if any
	GLint success;
	GLchar infoLog[512];

	glGetProgramiv(m_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl <<
			infoLog << std::endl;
	}

	// Delete shaders as they're linked into program and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{

}

void Shader::use() const
{
	glUseProgram(m_program);
}

void Shader::setUniform(const GLchar* const name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) const
{
	glUniform4f(glGetUniformLocation(m_program, name), x, y, z, w);
}

void Shader::setUniform(const GLchar* const name, const GLint i) const
{
	glUniform1i(glGetUniformLocation(m_program, name), i);
}

void Shader::setUniform(const GLchar* const name, const GLfloat* const matrix4) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE,  matrix4);
}

void Shader::setUniform(const GLuint id, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) const
{
	glUniform4f(id, x, y, z, w);
}

void Shader::setUniform(const GLuint id, const GLint i) const
{
	glUniform1i(id, i);
}

void Shader::setUniform(const GLuint id, const GLfloat* const matrix4) const
{
	glUniformMatrix4fv(id, 1, GL_FALSE, matrix4);
}

GLuint Shader::getUniformLocation(const GLchar* const name) const
{
	return glGetUniformLocation(m_program, name);
}

std::string Shader::getShaderText(const GLchar* const filePath) const
{
	std::ifstream file;

	// Ensure ifstream objects can throw exceptions
	file.exceptions(std::ifstream::badbit);

	try
	{
		file.open(filePath);
		std::stringstream fileStream;
		fileStream << file.rdbuf();
		file.close();
		return fileStream.str();
	}

	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		return std::string();
	}
}

GLuint Shader::createShader(const GLenum type, const GLchar* const shaderSource) const
{
	// Create shader
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	// Print any compiler errors
	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		std::string shaderType;

		switch (type)
		{
		case GL_VERTEX_SHADER:
			shaderType = "VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "FRAGMENT";
			break;
		default:
			shaderType = "UNKNOWN_TYPE";
			break;
		}

		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" + shaderType + "::COMPILAION_FAILED" << std::endl <<
			infoLog << std::endl;
	}

	return shader;
}