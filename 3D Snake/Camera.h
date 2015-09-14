#pragma once

#include <gl\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
public:
	Camera(const glm::vec3& position, const glm::vec3& rotation);
	~Camera();

	void setTarget(const glm::vec3 target, const GLboolean enableTargetLooking = GL_TRUE);
	void setLookAtTarget(const GLboolean enableTargetLooking);
	
	void moveRelative(const glm::vec3& moveAmount);
	void setPosition(const glm::vec3 position);

	void rotateRelative(const glm::vec3& rotation);
	void setRotation(const glm::vec3 rotation);
	void rotateAround(const glm::vec3 rotation, const GLfloat seconds);

	const GLboolean isRotating() const;

	const glm::vec3 getPosition() const;
	const glm::vec3 getRotation() const;
	
	const glm::vec3 getUp() const;
	const glm::vec3 getFront() const;
	const glm::vec3 getRight() const;

	const glm::mat4 getViewMatrix() const;

	void update(const GLfloat deltaTime);
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;

	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_up;
	
	glm::vec3 m_target;
	glm::vec3 m_wantedRotation;
	glm::vec3 m_rotationSum;

	GLfloat m_timeSum;
	GLfloat m_timeDuration;

	GLboolean m_lookAtTarget;
	
	GLboolean m_gettingToRotation;

	void updateVectors(const glm::vec3& rotation);
};
