#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count)
	: m_Count(count)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	/* This bind is what allows gl to draw. If not for this it wouldn't know what to draw*/
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	/* This hands over the data to the buffer */
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	/* This bind is what allows gl to draw. If not for this it wouldn't know what to draw*/
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	/* Unbinds the array buffer */
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
