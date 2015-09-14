#pragma once

#include <iostream>
#include <random>

#include <GL\glew.h>

#include <glm\glm.hpp>

class Util
{
public:
	// Rotates relative, so an angle of 0 results in no change
	static const glm::vec3 rotateVec3(const glm::vec3& vec, const glm::vec3& rot)
	{
		glm::mat4 rotMat(1);
		
		rotMat = glm::rotate(rotMat, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		rotMat = glm::rotate(rotMat, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		rotMat = glm::rotate(rotMat, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

		return glm::vec3(rotMat * glm::vec4(vec, 1.0f));
	}

	// Rotates around specified axiss
	static const glm::vec3 rotateVec3(const glm::vec3& vec, const glm::vec3& rot, const glm::vec3& xAxis, const glm::vec3& yAxis, const glm::vec3& zAxis)
	{
		glm::mat4 rotMat(1);

		rotMat = glm::rotate(rotMat, glm::radians(rot.x), xAxis);
		rotMat = glm::rotate(rotMat, glm::radians(rot.y), yAxis);
		rotMat = glm::rotate(rotMat, glm::radians(rot.z), zAxis);

		return glm::vec3(rotMat * glm::vec4(vec, 1.0f));
	}

	static const void print(const glm::vec3& vec)
	{
		std::cout << "x: " << vec.x << "\ty: " << vec.y << "\tz: " << vec.z << std::endl;
	}
};