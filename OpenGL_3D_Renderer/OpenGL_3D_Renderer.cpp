// OpenGL_3D_Renderer.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;

struct vec3d {
	float x, y, z;
};

struct triangle {
	vec3d points[3];
};

struct mesh {
	vector<triangle> tris;
};

struct mat4X4 {
	float m[4][4] = { 0 };
};

class Engine3D {
public:
	
	Engine3D()
	{
		
	}

private:
	mesh meshCube;
	mat4X4 matProj;
	float fTheta;

	void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4X4 &m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}

public:
	float ScreenHeight() {
		return 0.0f; // Garbage Function
	}
	float ScreenWidth() {
		return 0.0f; // Garbage Function
	}
	bool OnUserCreate()
	{
		meshCube.tris = {

			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};

		// Projection Matrix
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f); // 1 / tan(theta / 2) then convert degrees to radians

																	  // The matrix is initialized to 0 so only define changes
		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;

		return true;
	}
	bool OnUserUpdate(float elapsedTime)
	{
		//Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK); Paint black

		fTheta += 1.0f * elapsedTime;

		mat4X4 matRotZ, matRotX;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		for (auto tri : meshCube.tris) {
			triangle projected, translated, rotatedZ, rotatedZX;

			// Rotate in Z-Axis
			MultiplyMatrixVector(tri.points[0], rotatedZ.points[0], matRotZ);
			MultiplyMatrixVector(tri.points[1], rotatedZ.points[1], matRotZ);
			MultiplyMatrixVector(tri.points[2], rotatedZ.points[2], matRotZ);

			// Rotate in X-Axis
			MultiplyMatrixVector(rotatedZ.points[0], rotatedZX.points[0], matRotX);
			MultiplyMatrixVector(rotatedZ.points[1], rotatedZX.points[1], matRotX);
			MultiplyMatrixVector(rotatedZ.points[2], rotatedZX.points[2], matRotX);

			translated = rotatedZX;
			translated.points[0].z += tri.points[0].z + 4.50f;
			translated.points[1].z += tri.points[1].z + 4.50f;
			translated.points[2].z += tri.points[2].z + 4.50f;

			MultiplyMatrixVector(translated.points[0], projected.points[0], matProj);
			MultiplyMatrixVector(translated.points[1], projected.points[1], matProj);
			MultiplyMatrixVector(translated.points[2], projected.points[2], matProj);

			projected.points[0].x += 1.0f; projected.points[0].y += 1.0f;
			projected.points[1].x += 1.0f; projected.points[1].y += 1.0f;
			projected.points[2].x += 1.0f; projected.points[2].y += 1.0f;

			projected.points[0].x *= 0.5f * (float)ScreenWidth();
			projected.points[0].y *= 0.5f * (float)ScreenHeight();
			projected.points[1].x *= 0.5f * (float)ScreenWidth();
			projected.points[1].y *= 0.5f * (float)ScreenHeight();
			projected.points[2].x *= 0.5f * (float)ScreenWidth();
			projected.points[2].y *= 0.5f * (float)ScreenHeight();

			/*
			DrawTriangle(
				projected.points[0].x, projected.points[0].y,
				projected.points[1].x, projected.points[1].y,
				projected.points[2].x, projected.points[2].y,
				PIXEL_SOLID, FG_WHITE);
			*/
		}
		return true;
	}
};


struct ShaderSource {
	std::string VertexShader;
	std::string FragmentShader;
};


static ShaderSource ParseShader(const std::string& filePath) {
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
static unsigned int CompileShader(unsigned int type, const std::string& source) {

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
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

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

/**
int main()
{
Engine3D engine;
if (engine.ConstructConsole(256, 240, 4, 4)) {
engine.Start();
}
return 0;
}*/

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Needs to be defined after the glfw context is defined*/
	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	};

	std::cout << glGetString(GL_VERSION) << std::endl;

	/* Create a Triangle using a Vertex Buffer*/
	float tri[6] = {
		-0.5f, -0.5f,
		0.0f,  0.5f,
		0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);

	/* This bind is what allows gl to draw. If not for this it wouldn't know what to draw*/
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	/* This hands over the data to the buffer */
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), tri, GL_STATIC_DRAW);

	/* Super convoluted function to create an attribute for the position*/
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	/* Enable it*/
	glEnableVertexAttribArray(0);

	ShaderSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int program = CreateShader(source.VertexShader, source.FragmentShader);

	// Assign program that holds haders
	glUseProgram(program);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Draw the triangle */
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(program);

	glfwTerminate();
	return 0;
}

