#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

// GLEW for extension management
#define GLEW_STATIC
#include <GL\glew.h>

// GLFW for window management 
#include <GLFW\glfw3.h>

// SOIL for texture management
#include <SOIL\SOIL.h>

// GLM for matrix math
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Snake.h"
#include "Food.h"
#include "Util.h"

const int windowWidth = 500;
const int windowHeight = 500;

bool keys[1024];
Camera cam(glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f));
glm::vec3 newSnakeDir;

glm::vec3 snakeDirUp(0.0f, 1.0f, 0.0f);
glm::vec3 snakeDirDown(0.0f, -1.0f, 0.0f);
glm::vec3 snakeDirLeft(-1.0f, 0.0f, 0.0f);
glm::vec3 snakeDirRight(1.0f, 0.0f, 0.0f);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void calculateLocalSnakeDir(const glm::vec3& rotation);

int main()
{
	// Initialize glfw window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create glfw window object
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	// Initiliaze GLEW
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW\n";
		return -1;
	}

	// Setup OpenGL viewport
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Create shaders
	Shader shader("../3D Snake/vertex.vs", "../3D Snake/fragment.fs");

	// Vertices/Indices for Cube
	GLfloat vertices[] = 
	{
		 // Positions
							 
		 // Front			 
		 0.5f,  0.5f,  0.5f, // 0 Top Right
		 0.5f, -0.5f,  0.5f, // 1 Bottom Right
		-0.5f, -0.5f,  0.5f, // 2 Bottom Left
		-0.5f,  0.5f,  0.5f, // 3 Top Left 
							 
		 // Back			 
		-0.5f,  0.5f, -0.5f, // 4 Top Right
		-0.5f, -0.5f, -0.5f, // 5 Bottom Right
		 0.5f, -0.5f, -0.5f, // 6 Bottom Left
		 0.5f,  0.5f, -0.5f  // 7 Top Left 
	};

	GLuint indices[] = 
	{
		// Front
		0, 1, 3,
		1, 2, 3,

		// Back
		4, 5, 7,
		5, 6, 7,

		// Left
		3, 2, 4,
		2, 5, 4,

		// Right
		7, 6, 0,
		6, 1, 0,

		// Top
		7, 0, 4,
		0, 3, 4,

		// Bottom
		1, 6, 2,
		6, 5, 2
	};

	// Food stuffz
	GLint min = -16;
	GLint max = 16;
	Food food(min, max);

	// Snake stuffz
	const int initSnakeSize = 10;
	const Snake::Direction initSnakeDir = Snake::Direction::POS_Y;
	const glm::vec3 initSnakePos(0, 0, 0);
	Snake snake(initSnakePos, initSnakeDir, initSnakeSize, glm::vec3(0.0f));

	// Cubes stuffz
	const int cubeAmount = 0;
	const int halfCubeAmount = cubeAmount / 2;
	const int cubeAmountCubed = cubeAmount * cubeAmount * cubeAmount;
	//Cube cubes[cubeAmount][cubeAmount][cubeAmount];

	int modelMatrixLength = initSnakeSize + cubeAmountCubed;
	std::vector<glm::mat4> modelMatrices(modelMatrixLength);
	std::vector<glm::vec4> modelColors(modelMatrixLength);

	/*for (int i = 0; i < cubeAmount; i++)
	{
		for (int j = 0; j < cubeAmount; j++)
		{
			for (int k = 0; k < cubeAmount; k++)
			{
				cubes[i][j][k] = Cube(glm::vec3(i - halfCubeAmount, j - halfCubeAmount, k - halfCubeAmount));
				
				int matrixIndex = (i * cubeAmount * cubeAmount) + (j * cubeAmount) + k;
				modelMatrices[matrixIndex] = cubes[i][j][k].getModelMatrix();
				modelColors[matrixIndex] = glm::vec4((i * j) % 255, (j * k) & 255, (k * i * j) | 255, 1.0f);
			}
		}
	}*/

	for (int i = 0; i < snake.getLength(); i++)
	{
		modelMatrices[i + cubeAmountCubed] = snake.getBody(i).getModelMatrix();
		modelColors[i + cubeAmountCubed] = glm::vec4(snake.getColor(), 1.0f);
	}

	// Game VAO, VBO, EBO
	GLuint gameVAO;
	glGenVertexArrays(1, &gameVAO);
	glBindVertexArray(gameVAO);
		GLsizei vec4Size = sizeof(glm::vec4);

		GLuint modelMatrixInstanceVBO;
		glGenBuffers(1, &modelMatrixInstanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, modelMatrixInstanceVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelMatrixLength, &modelMatrices[0], GL_DYNAMIC_DRAW);
			// Instance model matrices
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint colorInstanceVBO;
		glGenBuffers(1, &colorInstanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorInstanceVBO);
			glBufferData(GL_ARRAY_BUFFER, vec4Size * modelMatrixLength, &modelColors[0], GL_DYNAMIC_DRAW);
			// Color
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);

			glVertexAttribDivisor(2, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		GLuint verticesVBO;
		glGenBuffers(1, &verticesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// Vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
			glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint indicesEBO;
		glGenBuffers(1, &indicesEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);

	// Matrix stuffz
	glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	// Time stuffz
	GLfloat prevTime = 0.0f;
	GLfloat currTime = 0.0f;
	GLfloat timeSum = 0.0f;
	GLfloat updateSpeed = 1 / 30.0f;

	cam.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	newSnakeDir = snake.getDirectionValue(initSnakeDir);

	// Program loop
	while (!glfwWindowShouldClose(window))
	{
		currTime = glfwGetTime();
		GLfloat deltaTime = currTime - prevTime;
		prevTime = currTime;
		timeSum += deltaTime;

		// Check and call events
		glfwPollEvents();
		cam.update(deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw objects
		shader.use();
		shader.setUniform("view", glm::value_ptr(cam.getViewMatrix()));
		shader.setUniform("projection", glm::value_ptr(projection));

		if (timeSum > updateSpeed)
		{
			timeSum = 0;

			snake.update(snake.getDirectionValue(newSnakeDir), food);

			modelMatrixLength = snake.getLength() + 1;
			modelMatrices.resize(modelMatrixLength);
			modelColors.resize(modelMatrixLength);

			for (int i = 0; i < snake.getLength(); i++)
			{
				modelMatrices[i + cubeAmountCubed] = snake.getBody(i).getModelMatrix();
				modelColors[i + cubeAmountCubed] = glm::vec4(snake.getColor(), 1.0f);
			}

			modelMatrices[modelMatrixLength - 1] = food.getModelMatrix();
			modelColors[modelMatrixLength - 1] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			//std::cout << cam.getPosition().x << " : " << cam.getPosition().y << " : " << cam.getPosition().z << std::endl;

			glBindBuffer(GL_ARRAY_BUFFER, modelMatrixInstanceVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelMatrixLength, &modelMatrices[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, colorInstanceVBO);
			glBufferData(GL_ARRAY_BUFFER, vec4Size * modelMatrixLength, &modelColors[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		glBindVertexArray(gameVAO);
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, modelMatrixLength);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &gameVAO);
	glDeleteBuffers(1, &verticesVBO);
	glDeleteBuffers(1, &indicesEBO);

	// Properly clean/delete all resources
	glfwTerminate();

	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		keys[key] = true;

		// Close window if esc was pressed
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, GL_TRUE);

		// Changing snake directions
		if (keys[GLFW_KEY_UP])
			newSnakeDir = snakeDirUp;

		if (keys[GLFW_KEY_DOWN])
			newSnakeDir = snakeDirDown;

		if (keys[GLFW_KEY_LEFT])
			newSnakeDir = snakeDirLeft;

		if (keys[GLFW_KEY_RIGHT])
			newSnakeDir = snakeDirRight;

		glm::vec3 rotate(0.0f);

		// Rotating
		if (keys[GLFW_KEY_W])
			rotate = glm::vec3(-90.0f, 0.0f, 0.0f);

		if (keys[GLFW_KEY_S])
			rotate = glm::vec3(90.0f, 0.0f, 0.0f);

		if (keys[GLFW_KEY_A])
			rotate = glm::vec3(0.0f, 90.0f, 0.0f);

		if (keys[GLFW_KEY_D])
			rotate = glm::vec3(0.0f, -90.0f, 0.0f);

		if (rotate != glm::vec3(0.0f) && cam.isRotating() == GL_FALSE)
		{
			cam.rotateAround(rotate, 0.25f);
			calculateLocalSnakeDir(rotate);
		}
	}

	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void calculateLocalSnakeDir(const glm::vec3& rotation)
{
	snakeDirUp = glm::normalize(Util::rotateVec3(snakeDirUp, rotation, cam.getRight(), cam.getUp(), cam.getFront()));
	snakeDirDown = glm::normalize(Util::rotateVec3(snakeDirDown, rotation, cam.getRight(), cam.getUp(), cam.getFront()));
	snakeDirLeft = glm::normalize(Util::rotateVec3(snakeDirLeft, rotation, cam.getRight(), cam.getUp(), cam.getFront()));
	snakeDirRight = glm::normalize(Util::rotateVec3(snakeDirRight, rotation, cam.getRight(), cam.getUp(), cam.getFront()));
}