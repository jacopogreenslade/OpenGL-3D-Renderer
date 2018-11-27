#shader VERTEX
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragmentPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	vec4 positionV4 = vec4(position, 1.0f);
	gl_Position = projection * view * model * positionV4;
	v_FragmentPosition = vec3(model * positionV4);
	v_Normal = mat3(transpose(inverse(model))) * normal;
};

#shader FRAGMENT
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragmentPosition;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec4 u_Color;

uniform vec3 lightPosition;

void main()
{
	float ambientIntencity = 0.2f;
	vec3 ambient = ambientIntencity * lightColor;

	vec3 normalizedNormal = normalize(v_Normal);
	vec3 lightDirection = normalize(lightPosition - v_FragmentPosition);
	
	float diff = max(dot(normalizedNormal, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;

	
	vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 1.0f);
};