#pragma once
#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextureCoordinates;
};

struct TextureS {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<TextureS> m_Textures;

	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<TextureS> textures);
	void Draw(Shader shader);
private:
	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};