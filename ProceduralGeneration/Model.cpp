#include "Model.h"




Model::Model()
{
}

Model::Model(std::string const & path, bool gamma)
{
	gammaCorrection = gamma;

}

void Model::AddNewMesh(std::vector<Vertex>& verts, std::vector<unsigned int>& inds, const char* vshaderPath, const char* fShaderPath)
{
	Mesh newMesh(verts, inds, vshaderPath, fShaderPath);
	meshes.push_back(newMesh);
}

void Model::AddNewMesh(Mesh mesh) {
	meshes.push_back(mesh);
}

void Model::SetTexture(unsigned int texture, Texture::TheOtherType num_type)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		this->meshes[i].meshMaterial->SetDiffuseTexture(texture, num_type);
		if (num_type == Texture::TheOtherType::Water)
			this->meshes[i].isWater = true;
	}
}

void Model::SetTexture(std::string texFile, Texture::TheOtherType num_type, bool transparent)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		this->meshes[i].meshMaterial->SetDiffuseTexture(texFile, num_type, transparent);
		if (num_type == Texture::TheOtherType::Water)
			this->meshes[i].isWater = true;
	}
}

void Model::SetTexture(std::vector<glm::vec4> imageData, Texture::TheOtherType num_type, bool transparent)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		this->meshes[i].meshMaterial->SetDiffuseTexture(imageData, num_type, transparent);
		if (num_type == Texture::TheOtherType::Water)
			this->meshes[i].isWater = true;
	}
}

//void Model::SetTextures(std::vector<std::string> texFiles)
//{
//	for (unsigned int i = 0; i < meshes.size(); i++) {
//		this->meshes[i].meshMaterial->SetDiffuseTexture(texFiles);
//	}
//}

void Model::SetSpecular(std::string texFile)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		this->meshes[i].meshMaterial->SetSpecularTexture(texFile);
	}
}

void Model::SetEmission(std::string texFile, Emission_Behaviour emB)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		this->meshes[i].meshMaterial->SetEmissionTexture(texFile, emB);
	}
}


Model::~Model()
{
}

void Model::Draw() const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw();
}
