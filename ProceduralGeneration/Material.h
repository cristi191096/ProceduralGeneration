#pragma once
#include <string>
#include <vector>
#include "glm.hpp"
#include "Shader.h"
#include "Texture.h"

enum Emission_Behaviour {
	NONE,
	FADE,
	TRANSLATE_UP,
	TRANSLATE_DOWN,
	TRANSLATE_LEFT,
	TRANSLATE_RIGHT
};

class Material
{
	int emissionAmplifier = 0;
	int emissionBehaviour;
	void SetShader(const char* vshaderPath, const char* fshaderPath);

public:
	Shader* shader;
	std::vector<Texture> textures;
	std::string name;

	glm::vec3 diffuseColour;
	glm::vec3 specularColour;
	//float dissolve; //Transparency
	//int illum;		//Illumination 
	float shininess;
	std::string diffTexName;
	std::string bumpMapName;
	//Texture* diffuseTexture;
	void SetDiffuseTexture(unsigned int textureid, Texture::TheOtherType num_type);
	void SetDiffuseTexture(std::string diffuseTexName, Texture::TheOtherType num_type, bool transparent = true);
	void SetDiffuseTexture(std::vector<glm::vec4> imageData, Texture::TheOtherType num_type, bool transparent = true);
	//void SetDiffuseTexture(std::vector<std::string> diffuseTexNames);
	void SetSpecularTexture(std::string specTexName);
	void SetEmissionTexture(std::string emissionTexName, Emission_Behaviour behave);
	//void PushMaterial(VertexBufferLayout* ly) const;
	Material(const char* vshaderPath = "Resources/Shaders/GameVertexShader.vs", const char* fShaderPath = "Resources/Shaders/GameFragmentShader.fs", float shine = 64.0f, glm::vec3 spec = glm::vec3(1.0f, 1.0f, 1.0f));
	Material(std::vector<Texture> tex, const char* vshaderPath = "Resources/Shaders/GameVertexShader.vs", const char* fShaderPath = "Resources/Shaders/GameFragmentShader.fs", float shine = 64.0f, glm::vec3 spec = glm::vec3(1.0f, 1.0f, 1.0f));
	Material(glm::vec3 colour, const char* vshaderPath = "Resources/Shaders/GameVertexShader.vs", const char* fShaderPath = "Resources/Shaders/GameFragmentShader.fs", float shine = 64.0f, glm::vec3 spec = glm::vec3(1.0f, 1.0f, 1.0f));
	~Material();

	void BindShader();
	void BindTextures();
	void UnindTextures();
	void BindUniforms();
};

