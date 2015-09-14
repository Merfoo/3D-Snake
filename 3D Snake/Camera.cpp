#include <iostream>

#include "Camera.h"
#include "Util.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation) :
	m_position(position),
	m_rotation(rotation),
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_right(glm::vec3(1.0f, 0.0f, 0.0f)),
	m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_target(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_lookAtTarget(GL_FALSE)
{
	updateVectors(m_rotation);
}

Camera::~Camera()
{
}

void Camera::setTarget(const glm::vec3 target, const GLboolean enableTargetLooking)
{
	m_target = target;
	m_lookAtTarget = enableTargetLooking;
}

void Camera::setLookAtTarget(const GLboolean enableTargetLooking)
{
	m_lookAtTarget = enableTargetLooking;
}

void Camera::moveRelative(const glm::vec3& moveAmount)
{
	m_position += m_front * moveAmount.z;
	m_position += m_up * moveAmount.y;
	m_position += m_right * moveAmount.x;
}

void Camera::setPosition(const glm::vec3 position)
{
	m_position = position;
}

void Camera::rotateRelative(const glm::vec3& rotation)
{
	updateVectors(rotation);
	m_rotation += rotation;
}

void Camera::setRotation(const glm::vec3 rotation)
{
	updateVectors(rotation - m_rotation);
	m_rotation = rotation;
}

void Camera::rotateAround(const glm::vec3 rotation, const GLfloat seconds)
{
	if (m_gettingToRotation == GL_TRUE)
		return;

	m_wantedRotation = rotation;
	m_rotationSum = glm::vec3(0.0f, 0.0f, 0.0f);
	m_timeSum = 0.0f;
	m_timeDuration = seconds;
	m_gettingToRotation = GL_TRUE;
}

const GLboolean Camera::isRotating() const
{
	return m_gettingToRotation;
}

const glm::vec3 Camera::getPosition() const
{
	return m_position;
}

const glm::vec3 Camera::getRotation() const
{
	return m_rotation;
}

const glm::vec3 Camera::getUp() const
{
	return m_up;
}

const glm::vec3 Camera::getFront() const
{
	return m_front;
}

const glm::vec3 Camera::getRight() const
{
	return m_right;
}

const glm::mat4 Camera::getViewMatrix() const
{
	if (m_lookAtTarget)
		return glm::lookAt(m_position, m_target - m_position, m_up);

	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::update(const GLfloat deltaTime)
{
	if (m_gettingToRotation == GL_TRUE)
	{
		glm::vec3 rotate = (m_wantedRotation - m_rotationSum) / glm::vec3((m_timeDuration - m_timeSum) / deltaTime);
		
		if (abs(m_rotationSum.x) >= abs(m_wantedRotation.x) && abs(m_rotationSum.y) >= abs(m_wantedRotation.y) && abs(m_rotationSum.z) >= abs(m_wantedRotation.z))
		{
			m_gettingToRotation = GL_FALSE;
			rotate = m_wantedRotation - m_rotationSum;
		}

		m_position = Util::rotateVec3(m_position - m_target, rotate, m_right, m_up, m_front) + m_target;
		m_rotationSum += rotate;
		m_timeSum += deltaTime;
		rotateRelative(rotate);
	}
}

void Camera::updateVectors(const glm::vec3& rotation)
{
	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

	m_front = glm::normalize(Util::rotateVec3(m_front, rotation, m_right, m_up, m_front));
	m_right = glm::normalize(Util::rotateVec3(m_right, rotation, m_right, m_up, m_front));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}