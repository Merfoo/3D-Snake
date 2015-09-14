#pragma once

#include <iostream>
#include <random>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "RandomNumber.h"

class Food
{
public:
	Food(const GLint min, const GLint max);
	~Food();

	const glm::vec3 getPosition() const;
	const glm::mat4 getModelMatrix() const;

	void generate();
private:
	RandomNumber m_random;
	glm::vec3 m_position;
};