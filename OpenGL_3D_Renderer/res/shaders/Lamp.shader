#shader VERTEX
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
};

#shader FRAGMENT
#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 lightColor;

void main()
{
	color = vec4(lightColor, 1.0f);
};