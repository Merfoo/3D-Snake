#pragma once

#include <iostream>
#include <deque>

#include <GL\glew.h>

#include <glm\glm.hpp>

#include "Cube.h"
#include "Food.h"

class Snake
{
public:
	enum class Direction { POS_X, POS_Y, POS_Z, NEG_X, NEG_Y, NEG_Z};

	Snake(const glm::vec3& startPos, const Direction& startDir, const GLuint startingSize, const glm::vec3& color);
	~Snake();

	void setDirection(const Direction& newDir);
	const Direction getDirection() const;
	const Direction getOppositeDirection(const Direction& dir) const;
	const glm::vec3 getDirectionValue(const Direction& dir) const;
	const Direction getDirectionValue(const glm::vec3& dir) const;

	const int getLength() const;
	const Cube getBody(const int i) const;
	const glm::vec3 getColor() const;

	void update(const Direction& newDir, Food& food);
private:
	Direction m_direction;
	glm::vec3 m_color;
	std::deque<Cube> m_body;
};
