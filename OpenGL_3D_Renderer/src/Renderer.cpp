#include "Renderer.h"
#include "iostream"

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "{OpenGL Error}" << error << "): " << function << "  " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	/* Clear Screen*/
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const {
	/* Bind all*/
	shader.Bind();
	vertexArray.Bind();
	indexBuffer.Bind();

	/*	Draw the triangle
			NOTE: you may want to change the unsigned int type
	*/
	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
	//GLCall(glDrawArrays(GL_TRIANGLES, 0, 8));

}