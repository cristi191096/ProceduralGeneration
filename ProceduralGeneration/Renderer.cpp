#include "Renderer.h"
#include "GameObject.h"
#include "VertexArray.h"
#include "IndexBuffer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Draw(VertexArray* va, IndexBuffer* ib, int vertsNum, DrawType type, bool wireframe) 
{

	//object->v_Array->Bind();
	/*object->mesh->indexBuffer->Bind();
	object->mesh->buffer->Bind();*/

	//if (wireframe) {
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_BACK, GL_LINE);
	//}
	va->Bind();
	ib->Bind();
	
	if (type == DrawType::ARRAYS) {
	glDrawElements(GL_TRIANGLE_STRIP,ib->GetCount(), GL_UNSIGNED_INT, nullptr);

	}
	else if (type == DrawType::ELEMENTS) {
		glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	else if (type == DrawType::LINES)
	{
		glDrawElements(GL_LINES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	va->Unbind();
	ib->Unbind();
	//object->v_Array->Unbind();
	/*object->mesh->indexBuffer->Unbind();
	object->mesh->buffer->Unbind();*/

	//if (wireframe) {
	//	glPolygonMode(GL_FRONT, GL_FILL);
	//	glPolygonMode(GL_BACK, GL_FILL);
	//}
}

void Renderer::Clear()
{
	glClearColor(0.5, 0.1, 0.12, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
