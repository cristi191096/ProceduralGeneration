#include "Mesh.h"
#include "Renderer.h"
#include "GameEngine.h"




Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, const char* vshaderPath, const char* fshaderPath) :
	vertices(verts), indices(inds)
{
	meshMaterial = new Material(vshaderPath, fshaderPath);
	VAO = new VertexArray();
	buffer = new VertexBuffer(verts, verts.size() * sizeof(Vertex));
	indexBuffer = new IndexBuffer(inds, inds.size());
	layout = new VertexBufferLayout();
	layout->Push<float>(3); //Position
	layout->Push<float>(3); //Normal
	layout->Push<float>(2); //UV
	layout->Push<float>(3); //Tangent
	layout->Push<float>(3); //Bitangent
	//VAO->Bind();
	//buffer->Bind();
	indexBuffer->Bind();
	VAO->AddBuffer(*buffer, *layout);
	VAO->Unbind();
	
}

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture> tex, const char* vshaderPath, const char* fshaderPath) :
	vertices(verts), indices(inds)
{
	meshMaterial = new Material(tex, vshaderPath,  fshaderPath);
	VAO = new VertexArray();
	buffer = new VertexBuffer(verts, verts.size() * sizeof(Vertex));
	indexBuffer = new IndexBuffer(inds, inds.size());
	layout = new VertexBufferLayout();
	layout->Push<float>(3);
	layout->Push<float>(3);
	layout->Push<float>(2);
	layout->Push<float>(3);
	layout->Push<float>(3);
	VAO->Bind();
	buffer->Bind();
	indexBuffer->Bind();
	VAO->AddBuffer(*buffer, *layout);
	VAO->Unbind();
	
}


void Mesh::Draw() const
{
//	glEnable(GL_CLIP_DISTANCE0);

	this->meshMaterial->BindShader();

	this->meshMaterial->BindTextures();
	
//	glDisable(GL_CLIP_DISTANCE0);
	if (isTree)
	{
		Renderer::Draw(VAO, indexBuffer, vertices.size(), DrawType::LINES, true);
	}
	else
	{
		Renderer::Draw(VAO, indexBuffer, vertices.size(), DrawType::ARRAYS, true);
	}
	
	this->meshMaterial->UnindTextures();
	glActiveTexture(GL_TEXTURE0);
	

}

Mesh::~Mesh()
{
	//if (this != nullptr) {
	//	delete buffer;
	//	delete indexBuffer;
	//	delete layout;
	//	delete meshMaterial;
	//}
	//else
	//{
	//	buffer = nullptr;
	//	indexBuffer = nullptr;
	//	layout = nullptr;
	//	meshMaterial = nullptr;
	//}
}

