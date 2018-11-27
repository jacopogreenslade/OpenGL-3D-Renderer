#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include "Renderer.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureS> textures)
	: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
	SetupMesh();
}

void Mesh::Draw(Shader shader)
{
}

void Mesh::SetupMesh()
{
	/*
	VertexArray va;
	// VertexBuffer vb(vertexPositions, 6 * 8 * sizeof(float)); // includes normals etc
	VertexBuffer vb(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
	VertexBufferLayout layout;
	layout.Push<Vertex>(3); // vertices
	layout.Push<Vertex>(3); // normals
	layout.Push<Vertex>(2); // texture coordinates
	va.AddBuffer(vb, layout);

	IndexBuffer ib(&m_Indices[0], m_Indices.size());


	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	shader.SetUniformMat4f("u_MVP", matProj); // Fixed
	shader.SetUniformMat4f("u_rot", mat_yAxisRot * Translate(0, 0, -5));

	Texture texture("res/textures/Wartortle.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);
	*/

	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenBuffers(1, &EBO));

	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

	GLCall(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
		&m_Indices[0], GL_STATIC_DRAW));

	// vertex positions
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	// vertex normals
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
	// vertex texture coords
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoordinates)));

	GLCall(glBindVertexArray(0));
}
