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

#include "Camera.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

using namespace std;

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

glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPosition = glm::vec3(-4.0f, 1.0f, -3.0f);

glm::vec3 cubePosition = glm::vec3(0.0f, 2.0f, 0.0f);



int main(void)
{
	GLFWwindow* window;

	int ScreenWidth = 800;
	int ScreenHeight = 600;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(ScreenWidth, ScreenHeight, "3D Renderer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Should limit the framerate*/
	glfwSwapInterval(1);

	/* Needs to be defined after the glfw context is defined*/
	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	};

	// configure global opengl state
	// -----------------------------
	glEnable(GL_BLEND);

	/* This makes sure that the z-depth is displayed correcly
			Without it, normals might appear inverted.
			For it to work properly, you MUST CLEAR TEH DEPTH BUFFER!!!
			That's when you call glClear(GL_DEPTH_BUFFER_BIT);
			We do it in renderer.clear();
	*/
	glEnable(GL_DEPTH_TEST);

	std::cout << glGetString(GL_VERSION) << std::endl;

	/*Create scope to avoid app not closing*/
	{
		// Throw it in a method or it won't work
		float angle = 0.0f;
		glm::mat4 mat_yAxisRot = RotateY(angle);

		//	glm::mat4 matProj = glm::ortho(-10.0f, 10.0f, -7.5f, 7.5f, -1.0f, 1.0f);
		/*float vertices[] = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};
*/
		float vertices[216] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};
		/*

		The matrix is initialized to 0 so only define changes
		This is what is going on inside the matrix above

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
		*/
		/* Create a Triangle using a Vertex Buffer*/
		//float vertexPositions[] = {
		//	-2.0f, -2.0f, -5.0f, /*UV*/ 0.0f, 0.0f, /* Normals */  0.0f, 0.0f, 1.0f,  /* 0 bottom L */
		//	2.0f, -2.0f, -5.0f, /*UV*/ 1.0f, 0.0f, /* Normals */  0.0f, 0.0f, 1.0f, /* 1 bottom R */
		//	2.0f,  2.0f, -5.0f, /*UV*/ 1.0f, 1.0f, /* Normals */  0.0f, 0.0f, 1.0f, /* 2 top R		*/

		//	2.0f,  2.0f, -5.0f, /*UV*/ 1.0f, 1.0f, /* Normals */  0.0f, 0.0f, 1.0f, /* 2 top R		*/
		//	-2.0f,  2.0f, -5.0f, /*UV*/ 0.0f, 1.0f, /* Normals */  0.0f, 0.0f, 1.0f, /* 3 top L		*/
		//	-2.0f, -2.0f, -5.0f, /*UV*/ 0.0f, 0.0f, /* Normals */	0.0f, 0.0f, 1.0f  /* 0 bottom L */
		//};

		int numberOfValues = 6 * 6;
		unsigned int *indeces = new unsigned int[numberOfValues];

		// populate index array
		for (unsigned int i = 0; i < numberOfValues; i++)
			indeces[i] = i;
		
		Camera camera(glm::vec3(2.0f, 2.0f, 1.0f));

		// Cube
		VertexArray cubeVAO;
		VertexBuffer cubeVBO(vertices, 216 * sizeof(float));
		VertexBufferLayout cubeLayout;
		cubeLayout.Push<float>(3);
		cubeLayout.Push<float>(3);
		cubeVAO.AddBuffer(cubeVBO, cubeLayout);

		Shader lightingShader("res/shaders/Lighting.shader");
		lightingShader.Bind();
		glm::vec3 modelColor = glm::vec3(1.0f, 0.0f, 0.3f);
		lightingShader.SetUniformVec3("lightColor", lightColor);
		lightingShader.SetUniformVec3("objectColor", modelColor);

		// Light
		VertexArray lightVAO;
		VertexBuffer lightVBO(vertices, 6 * 6 * 6 * sizeof(float));
		VertexBufferLayout lightLayout;
		lightLayout.Push<float>(3);
		lightLayout.Push<float>(3);
		// lightLayout.SetStride(6 * sizeof(float));
		lightVAO.AddBuffer(lightVBO, lightLayout);

		Shader lampShader("res/shaders/Lamp.shader");
		lampShader.Bind();
		lampShader.SetUniformVec3("lightColor", lightColor);

		// Can use the same for both
		IndexBuffer ib(indeces, 36);

		// Projection Matrix
		float fNear = 0.1f;
		float fFar = 100.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight / (float)ScreenWidth;
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f); // 1 / tan(theta / 2) then convert degrees to radians
		glm::mat4 matProj = glm::perspective(fFov, fAspectRatio, fNear, fFar);
		glm::mat4 model;

		/*
		Texture texture("res/textures/Wartortle.png");
		texture.Bind();
		lightingShader.SetUniform1i("u_Texture", 0);


		/* ERROR: {1282} - Having the lightingShader uniform binding after te unbinds below is an issue */

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		float red = 1.0f;
		float increment = 0.05f;
		float angleIncrement = 0.05f;
		float lastFrame = 0.0f;

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			camera.ProcessInput(window, deltaTime);
			
			renderer.Clear();

			// Render the CUBE
			lightingShader.Bind();
			lightingShader.SetUniformMat4f("projection", matProj); // Projection 
			lightingShader.SetUniformMat4f("view", camera.GetViewMatrix());
			lightingShader.SetUniformMat4f("model", glm::translate(glm::mat4(), cubePosition)); // Model
			lightingShader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
			lightingShader.SetUniformVec3("lightPosition", lightPosition);
			// This takes care of binding the VAO and IBO
			renderer.Draw(cubeVAO, ib, lightingShader);

			// Render the LAMP
			lampShader.Bind();
			lampShader.SetUniformMat4f("projection", matProj); // Projection
			lampShader.SetUniformMat4f("view", camera.GetViewMatrix());
			model = glm::mat4();
			model = glm::translate(model, lightPosition);
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.SetUniformMat4f("model", model); // Model
			lampShader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
			// This takes care of binding the VAO and IBO
			renderer.Draw(lightVAO, ib, lampShader);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		

		ib.Unbind();
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

