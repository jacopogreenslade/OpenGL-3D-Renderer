#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

#include <iostream>

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer & vb, VertexBufferLayout layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		std::cout << "Element " << i << " is type " << element.type << "" << std::endl;
		
		/* Enable Attribute*/
		GLCall(glEnableVertexAttribArray(i));
		/*Attributes are set per vertex*/
		/* Super convoluted function to create an attribute for the position*/
		GLCall(glVertexAttribPointer(i, 
			element.count,
			element.type,
			element.normalized,
			layout.GetStride(),
			(const void*) offset));
		/* Increment offset by elemnet count*/
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}
