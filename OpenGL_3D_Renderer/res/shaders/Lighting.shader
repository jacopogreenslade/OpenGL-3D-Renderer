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
uniform vec3 viewPosition;

void main()
{
	// Ambient
	float ambientIntencity = 0.2f;
	vec3 ambient = ambientIntencity * lightColor;
	
	// Diffuse
	vec3 normalizedNormal = normalize(v_Normal);
	vec3 lightDirection = normalize(lightPosition - v_FragmentPosition);

	float diff = max(dot(normalizedNormal, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specularity
	float specularIntensity = 0.8f;

	vec3 viewDirection = normalize(viewPosition - v_FragmentPosition);
	// Reflect the light directiona around the normal
	vec3 reflectedLightDirection = reflect(-lightDirection, normalizedNormal);
	// For normalized values [0, 1] the higher the power it is raised to the smaller the result;
	// The greater the shine the more diffused the specularity is.
	float shine = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), 32);
	vec3 specular = specularIntensity * shine * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0f);
};