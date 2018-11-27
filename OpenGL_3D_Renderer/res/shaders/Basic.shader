#shader VERTEX
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_Normal;

uniform mat4 u_MVP;
uniform mat4 u_rot;

void main() 
{
	gl_Position = u_MVP * u_rot * position;
	v_TexCoord = texCoord;
	v_Normal = vec3(u_rot * vec4(normal, 1.0f));
};

#shader FRAGMENT
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	vec3 norm = normalize(v_Normal);
	float diff = max(dot(norm, vec3(0.0f, -1.0f, -1.0f)), 0.0f);
	
	vec4 result = (diff * vec4(1.0f, 1.0f, 1.0f, 1.0f)) * texColor;
	color = u_Color;
};