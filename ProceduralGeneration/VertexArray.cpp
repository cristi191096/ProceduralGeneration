#include "VertexArray.h"



VertexArray::VertexArray()
{
	glGenVertexArrays(1, &arrayID);
	
}


VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &arrayID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout & layout)
{
	Bind();
	vb.Bind();
	unsigned int offset = 0;
	const auto& elements = layout.GetElements();
	for (unsigned int i = 0; i < elements.size();i++) {
		glEnableVertexAttribArray(i);
		const auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(), (const void*)offset);
		offset += element.count * VBElements::GetTypeSize(element.type);
	}


}

void VertexArray::Bind() const
{
	glBindVertexArray(arrayID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
