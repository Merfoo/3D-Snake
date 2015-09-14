#pragma once

#include <glm\glm.hpp>

class Cube
{
public:
	Cube();
	Cube(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));
	~Cube();

	void setPosition(const glm::vec3 position);
	void setRotation(const glm::vec3 rotation);
	void setScale(const glm::vec3 scale);
	void setColor(const glm::vec3 color);

	const glm::vec3 getPosition() const;
	const glm::vec3 getColor() const;

	const glm::mat4 getModelMatrix() const;
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	glm::vec3 m_color;
};