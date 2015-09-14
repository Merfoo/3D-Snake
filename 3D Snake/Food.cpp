#include "Food.h"

Food::Food(const GLint min, const GLint max) :
	m_random(min, max)
{
	generate();
}

Food::~Food()
{
}

const glm::vec3 Food::getPosition() const
{
	return m_position;
}

const glm::mat4 Food::getModelMatrix() const
{
	glm::mat4 model;

	model = glm::translate(model, m_position);

	return model;
}

void Food::generate()
{
	m_position = glm::vec3(m_random.get(), m_random.get(), m_random.get());

	std::cout << "Food Generate Called\n";
}