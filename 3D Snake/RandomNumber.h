#pragma once

#include <iostream>
#include <random>

#include <GL\glew.h>

class RandomNumber
{
public:
	RandomNumber(const GLint min, const GLint max);
	~RandomNumber();

	const GLint get();
private:
	std::random_device m_rd;
	std::mt19937 m_eng;
	std::uniform_int<GLint> m_dist;
};