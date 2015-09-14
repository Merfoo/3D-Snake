#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL\glew.h>

class Shader
{
public:
	Shader(const GLchar* const vertexPath, const GLchar* const fragmentPath);
	~Shader();

	void use() const;

	void setUniform(const GLchar* const name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) const;
	void setUniform(const GLchar* const name, const GLint i) const;
	void setUniform(const GLchar* const name, const GLfloat* const matrix4) const;

	void setUniform(const GLuint id, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) const;
	void setUniform(const GLuint id, const GLint i) const;
	void setUniform(const GLuint id, const GLfloat* const matrix4) const;

	GLuint getUniformLocation(const GLchar* const name) const;
private:
	GLuint m_program;

	std::string getShaderText(const GLchar* const filePath) const;
	GLuint createShader(const GLenum type, const GLchar* const shaderSource) const;
};