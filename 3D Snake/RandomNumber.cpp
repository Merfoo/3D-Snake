#include "RandomNumber.h"

RandomNumber::RandomNumber(const GLint min, const GLint max) :
	m_rd(),
	m_eng(m_rd()),
	m_dist(min, max)
{
}

RandomNumber::~RandomNumber()
{
}

const GLint RandomNumber::get()
{
	return m_dist(m_eng);
}