#include "Snake.h"

#include "Util.h"

Snake::Snake(const glm::vec3& startPos, const Direction& startDir, const GLuint startingSize, const glm::vec3& color) :
	m_direction(startDir),
	m_color(color),
	m_body(startingSize)
{
	const glm::vec3 dirVal = getDirectionValue(getOppositeDirection(startDir));

	m_body[0] = Cube(startPos, glm::vec3(), glm::vec3(1));

	for (int i = 1; i < startingSize; i++)
		m_body[i] = Cube(m_body[i - 1].getPosition() + dirVal);
}

Snake::~Snake()
{

}

void Snake::setDirection(const Direction& newDir)
{
	m_direction = newDir;
}

const Snake::Direction Snake::getDirection() const
{
	return m_direction;
}

const Snake::Direction Snake::getOppositeDirection(const Direction& dir) const
{
	switch (dir)
	{
	case Snake::Direction::POS_X:
		return Snake::Direction::NEG_X;
		break;
	case Snake::Direction::POS_Y:
		return Snake::Direction::NEG_Y;
		break;
	case Snake::Direction::POS_Z:
		return Snake::Direction::NEG_Z;
		break;
	case Snake::Direction::NEG_X:
		return Snake::Direction::POS_X;
		break;
	case Snake::Direction::NEG_Y:
		return Snake::Direction::POS_Y;
		break;
	case Snake::Direction::NEG_Z:
		return Snake::Direction::POS_Z;
		break;
	}
}

const glm::vec3 Snake::getDirectionValue(const Direction& dir) const
{
	const int dirMag = 1;

	switch (dir)
	{
	case Snake::Direction::POS_X:
		return glm::vec3(dirMag, 0, 0);
		break;
	case Snake::Direction::POS_Y:
		return glm::vec3(0, dirMag, 0);
		break;
	case Snake::Direction::POS_Z:
		return glm::vec3(0, 0, dirMag);
		break;
	case Snake::Direction::NEG_X:
		return glm::vec3(-dirMag, 0, 0);
		break;
	case Snake::Direction::NEG_Y:
		return glm::vec3(0, -dirMag, 0);
		break;
	case Snake::Direction::NEG_Z:
		return glm::vec3(0, 0, -dirMag);
		break;
	}
}

const Snake::Direction Snake::getDirectionValue(const glm::vec3& dir) const
{
	const glm::vec3 epsilon(0.5f, 0.5f, 0.5f);

	if (dir.x > epsilon.x)
		return Snake::Direction::POS_X;

	else if (dir.x < -epsilon.x)
		return Snake::Direction::NEG_X;

	else if (dir.y > epsilon.y)
		return Snake::Direction::POS_Y;

	else if (dir.y < -epsilon.y)
		return Snake::Direction::NEG_Y;

	else if (dir.z > epsilon.z)
		return Snake::Direction::POS_Z;

	return Snake::Direction::NEG_Z;
}

const int Snake::getLength() const
{
	return m_body.size();
}

const Cube Snake::getBody(const int i) const
{
	return m_body[i];
}

const glm::vec3 Snake::getColor() const
{
	return m_color;
}

void Snake::update(const Direction& newDir, Food& food)
{
	if (m_direction != newDir && m_direction != getOppositeDirection(newDir))
		m_direction = newDir;

	m_body.push_front(Cube(getDirectionValue(m_direction) + m_body[0].getPosition()));

	Util::print(m_body[0].getPosition());
	Util::print(food.getPosition());

	if (m_body[0].getPosition() != food.getPosition())
		m_body.pop_back();

	else
		food.generate();
}