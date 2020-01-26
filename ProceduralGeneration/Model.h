#pragma once
#include "Mesh.h"


#include <iostream>
#include <string>
#include <vector>

class Model
{

public:
	std::string directory;
	std::vector<Mesh> meshes;
	bool gammaCorrection;
	Model();
	Model(std::string const &path, bool gamma = false);
	void AddNewMesh(std::vector<Vertex>& verts, std::vector<unsigned int>& inds, const char* vshaderPath = "Resources/Shaders/GameVertexShader.vs", const char* fShaderPath = "Resources/Shaders/GameFragmentShader.fs");
	void AddNewMesh(Mesh mesh);
	void SetTexture(unsigned int texture, Texture::TheOtherType num_type);
	void SetTexture(std::string texFile, Texture::TheOtherType num_type, bool transparent = true);
	void SetTexture(std::vector<glm::vec4> imageData, Texture::TheOtherType num_type, bool transparent = true);
	//void SetTextures(std::vector<std::string> texFiles);
	void SetSpecular(std::string texFile);
	void SetEmission(std::string texFile, Emission_Behaviour emB);
	~Model();

	void Draw() const;
};

