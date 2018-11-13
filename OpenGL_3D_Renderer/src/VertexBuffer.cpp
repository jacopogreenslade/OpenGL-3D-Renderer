#include "VertexBuffer.h"
#include "Renderer.h"


VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	/* This bind is what allows gl to draw. If not for this it wouldn't know what to draw*/
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	/* This hands over the data to the buffer */
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	/* This bind is what allows gl to draw. If not for this it wouldn't know what to draw*/
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	/* Unbinds the array buffer */
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
