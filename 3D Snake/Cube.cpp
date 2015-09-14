#include <glm\gtc\matrix_transform.hpp>

#include "Cube.h"

Cube::Cube()
{
	// Warning, does not actually initialize the member variables, weird
	//Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}

Cube::Cube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
	m_position(position),
	m_rotation(rotation),
	m_scale(scale),
	m_color(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Cube::~Cube()
{
}

void Cube::setPosition(const glm::vec3 position)
{
	m_position = position;
}

void Cube::setRotation(const glm::vec3 rotation)
{
	m_rotation = rotation;
}

void Cube::setScale(const glm::vec3 scale)
{
	m_scale = scale;
}

void Cube::setColor(const glm::vec3 color)
{
	m_color = color;
}

const glm::vec3 Cube::getPosition() const
{
	return m_position;
}

const glm::vec3 Cube::getColor() const
{
	return m_color;
}

const glm::mat4 Cube::getModelMatrix() const
{
	glm::mat4 model;
	
	model = glm::scale(model, m_scale);

	model = glm::rotate(model, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	
	model = glm::translate(model, m_position);

	return model;
}