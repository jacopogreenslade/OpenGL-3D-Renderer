// OpenGL_3D_Renderer.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Shader.h"
#include "Texture.h"

#include "Renderer.h"

#include "ObjectImporter.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

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

static glm::mat4 RotateY(float angle) {
	float yAxisRotationArray[] = {
		cosf(angle),  0.0f, sin(angle),  0.0f,
		0.0f,				  1.0f, 0.0f,				 0.0f,
		-sinf(angle), 0.0f, cosf(angle), 0.0f,
		0.0f,				  0.0f, 0.0f,				 1.0f,
	};

	// The y-axis rotation matrix is
	return glm::make_mat4(yAxisRotationArray);
}

static glm::mat4 RotateYAroundPosition(float angle, float x, float y, float z) {

	glm::mat4 moveToCenter = glm::mat4();
	moveToCenter[3][0] = -x;
	moveToCenter[3][1] = -y;
	moveToCenter[3][2] = -z;

	glm::mat4 moveBack = glm::mat4();
	moveBack[3][0] = x;
	moveBack[3][1] = y;
	moveBack[3][2] = z;

	// Rotation around 0
	glm::mat4 yAxisRotation = glm::mat4();
	yAxisRotation[0][0] = cosf(angle);
	yAxisRotation[0][2] = -sinf(angle);
	yAxisRotation[2][0] = sinf(angle);
	yAxisRotation[2][2] = cosf(angle);

	// The y-axis rotation matrix is
	return moveBack * yAxisRotation * moveToCenter;
}

static glm::mat4 Translate(float x, float y, float z) {
	glm::mat4 mov = glm::mat4();
	mov[3][0] = x;
	mov[3][1] = y;
	mov[3][2] = z;

	return mov;
}
//
//int main(void)
//{
//	GLFWwindow* window;
//
//	int ScreenWidth = 640;
//	int ScreenHeight = 480;
//
//	/* Initialize the library */
//	if (!glfwInit())
//		return -1;
//
//	/* Create a windowed mode window and its OpenGL context */
//	window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Hello World", NULL, NULL);
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//
//	/* Make the window's context current */
//	glfwMakeContextCurrent(window);
//
//	/* Should limit the framerate*/
//	glfwSwapInterval(1);
//
//	/* Needs to be defined after the glfw context is defined*/
//	if (glewInit() != GLEW_OK) {
//		std::cout << "Error!" << std::endl;
//	};
//
//	std::cout << glGetString(GL_VERSION) << std::endl;
//
//	/*Create scope to avoid app not closing*/
//	{
//		// Projection Matrix
//		float fNear = 0.1f;
//		float fFar = 10.0f;
//		float fFov = 90.0f;
//		float fAspectRatio = (float)ScreenHeight / (float)ScreenWidth;
//		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f); // 1 / tan(theta / 2) then convert degrees to radians
//
//		glm::mat4 matProj = glm::perspective(fFov, fAspectRatio, fNear, fFar);
//
//		// Throw it in a method or it won't work
//		float angle = 0.0f;
//		glm::mat4 mat_yAxisRot = RotateY(angle);
//
////	glm::mat4 matProj = glm::ortho(-10.0f, 10.0f, -7.5f, 7.5f, -1.0f, 1.0f);
//
//		/* 
//		
//		The matrix is initialized to 0 so only define changes
//		This is what is going on inside the matrix above
//
//		matProj.m[0][0] = fAspectRatio * fFovRad;
//		matProj.m[1][1] = fFovRad;
//		matProj.m[2][2] = fFar / (fFar - fNear);
//		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
//		matProj.m[2][3] = 1.0f;
//		matProj.m[3][3] = 0.0f;
//		*/
//		/* Create a Triangle using a Vertex Buffer*/
//		float vertexPositions[] = {
//			 -2.0f, -2.0f, -5.0f, /*UV*/ 0.0f, 0.0f, /* Normals */  0.0f, 0.0f, 1.0f,  /* 0 bottom L */ 
//			  2.0f, -2.0f, -5.0f, /*UV*/ 1.0f, 0.0f, /* Normals */  0.0f, 0.0f, 1.0f, /* 1 bottom R */
//			  2.0f,  2.0f, -5.0f, /*UV*/ 1.0f, 1.0f, /* Normals */  0.0f, 0.0f, 1.0f, /* 2 top R		*/
//
//				2.0f,  2.0f, -5.0f, /*UV*/ 1.0f, 1.0f, /* Normals */  0.0f, 0.0f, 1.0f, /* 2 top R		*/
//			 -2.0f,  2.0f, -5.0f, /*UV*/ 0.0f, 1.0f, /* Normals */  0.0f, 0.0f, 1.0f, /* 3 top L		*/
//			 -2.0f, -2.0f, -5.0f, /*UV*/ 0.0f, 0.0f, /* Normals */	0.0f, 0.0f, 1.0f  /* 0 bottom L */
//		};
//
//		std::string path = "cube.obj";
//		std::vector<float> vertPos = ObjectImporter::ImportFromFile(path);
//
//		int size = vertPos.size();
//		float *vertPosArray = new float[size];
//		std::copy(vertPos.begin(), vertPos.end(), vertPosArray);
//
//		std::cout << "size: " << size << std::endl;
//
//		for (int i = 0; i < size - 2; i++) {
//			
//		}
//
//		unsigned int indeces[] = {
//			0, 1, 2,
//			3, 4, 5
//		};
//
//		GLCall(glEnable(GL_BLEND));
//		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//	
//		VertexArray va;
//		// VertexBuffer vb(vertexPositions, 6 * 8 * sizeof(float)); // includes normals etc
//		VertexBuffer vb(vertPosArray, size * 3 * sizeof(float));
//		VertexBufferLayout layout;
//		layout.Push<float>(3);
//		//layout.Push<float>(2);
//		//layout.Push<float>(3);
//		va.AddBuffer(vb, layout);
//
//		IndexBuffer ib(indeces, size);
//
//		Shader shader("res/shaders/Basic.shader");
//		shader.Bind();
//
//		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
//		shader.SetUniformMat4f("u_MVP", matProj); // Fixed
//		shader.SetUniformMat4f("u_rot", mat_yAxisRot * Translate(0, 0, -5));
//
//		Texture texture("res/textures/Wartortle.png");
//		texture.Bind();
//		shader.SetUniform1i("u_Texture", 0);
//
//		/* ERROR: {1282} - Having the shader uniform binding after teh unbinds below is an issue */
//
//		va.Unbind();
//		shader.Unbind();
//		vb.Unbind();
//		ib.Unbind();
//
//		Renderer renderer;
//
//		ImGui::CreateContext();
//		ImGui_ImplGlfwGL3_Init(window, true);
//		ImGui::StyleColorsDark();
//
//
//		float red = 1.0f;
//		float increment = 0.05f;
//		float angleIncrement = 0.05f;
//		
//		bool show_demo_window = true;
//		bool show_another_window = false;
//		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//		/* Loop until the user closes the window */
//		while (!glfwWindowShouldClose(window))
//		{
//			renderer.Clear();
//
//			ImGui_ImplGlfwGL3_NewFrame();
//
//			/* Bind Shader Program*/
//			shader.Bind();
//
//			/* Change color every frame*/
//			shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);
//			shader.SetUniformMat4f("u_rot", RotateYAroundPosition(angle, 0, 0, -5.0f) * Translate(0, 0, -5));
//
//			renderer.Draw(va, ib, shader);
//
//			/*Update the color*/
//			if (red > 1.0f) {
//				increment = -0.05f;
//			}
//			else if (red < 0.0f) {
//				increment = 0.05f;
//			}
//
//			if (angle + angleIncrement > 360) { angle = 0.0f; }
//			angle += angleIncrement;
//			red += increment;
//
//			// 1. Show a simple window.
//			// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
//			{
//				//static float f = 0.0f;
//                          // Display some text (you can use a format string too)
//				//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
//				//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//				//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
//				//if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
//				// ImGui::Text("counter = %d", counter);
//
//				//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//			}
//
//			ImGui::Render();
//			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
//
//			/* Swap front and back buffers */
//			glfwSwapBuffers(window);
//
//			/* Poll for and process events */
//			glfwPollEvents();
//		}
//
//		delete[] vertPosArray;
//	}
//	ImGui_ImplGlfwGL3_Shutdown();
//	ImGui::DestroyContext();
//	glfwTerminate();
//	return 0;
//}

