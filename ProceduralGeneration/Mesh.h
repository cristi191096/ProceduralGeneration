#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "glm.hpp"
#include "gtc\constants.hpp"



struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 biTangent;
};

class Mesh
{
	

public:
	Material* meshMaterial;
	VertexBufferLayout* layout;
	VertexBuffer* buffer;
	IndexBuffer* indexBuffer;
	VertexArray* VAO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices/* = { 0 }*/;
	std::string name;
	bool isWater = false;
	bool isTree = false;

	Mesh();
	Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, const char* vshaderPath = "Resources/Shaders/GameVertexShader.vs", const char* fShaderPath = "Resources/Shaders/GameFragmentShader.fs");
	Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture> tex, const char* vshaderPath = "Resources/Shaders/GameVertexShader.vs", const char* fShaderPath = "Resources/Shaders/GameFragmentShader.fs");
	void Draw() const;
	~Mesh();

};

