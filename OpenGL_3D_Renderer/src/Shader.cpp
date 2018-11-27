#include "Shader.h"
#include "Renderer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

Shader::Shader(const std::string & filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
	ShaderSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

ShaderSource Shader::ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath); // Explicit constructor syntax
	std::string line;
	std::stringstream ss[2];

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			// We've found #shader and need to decide what type of shader we're dealing with
			if (line.find("VERTEX") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("FRAGMENT") != std::string::npos) {
				type = ShaderType::FRAGMENT;

			}

		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	// Temporarily removed GLCall to avoid 1282 error breakpoint
	//glUniform1i(GetUniformLocation(name), value);

	GLCall(glUniform1i(GetUniformLocation(name), value));
}

/*Uniforms are set per draw. Must be after the shaders are set.*/
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

/*Uniforms are set per draw. Must be after the shaders are set.*/
void Shader::SetUniformVec3(const std::string& name, glm::vec3 v)
{
	GLCall(glUniform3fv(GetUniformLocation(name), 1, &v[0]));
}

/*Uniforms are set per draw. Must be after the shaders are set.*/
void Shader::SetUniformVec4(const std::string& name, glm::vec4 v)
{
	GLCall(glUniform4fv(GetUniformLocation(name), 1, &v[0]));
}

void Shader::SetUniformMat4f(const std::string & name, const glm::mat4& matrix)
{
	/*&matrix[0][0] is the address of the first element*/
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: Uniform " << name.c_str() << " doesnt exist!" << std::endl;

	std::cout << "INFO: Uniform " << name.c_str() << " has location "<< location << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}