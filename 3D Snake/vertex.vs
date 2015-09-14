#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec4 color;
layout (location = 3) in mat4 model;

out vec4 newColor;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	newColor = color;
}