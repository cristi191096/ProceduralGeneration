#include "Material.h"
#include "VertexBufferLayout.h"
#include "GameEngine.h"


void Material::SetShader(const char * vshaderPath, const char * fshaderPath)
{
	if (GameEngine::currentShader == nullptr) {
		shader = new Shader(vshaderPath, fshaderPath);
		GameEngine::currentShader = shader;
		GameEngine::allShaders.push_back(shader);
		//shader->Bind();
	}
	else
	{
		if (vshaderPath != GameEngine::currentShader->vertexPath && fshaderPath != GameEngine::currentShader->fragmentPath) {
			for (int i = 0; i < GameEngine::allShaders.size(); i++) {
				if (vshaderPath == GameEngine::allShaders[i]->vertexPath && fshaderPath == GameEngine::allShaders[i]->fragmentPath) {
					GameEngine::currentShader = GameEngine::allShaders[i];
					shader = GameEngine::currentShader;
					break;
				}
			}
			if (shader == nullptr) {

				shader = new Shader(vshaderPath, fshaderPath);
				GameEngine::currentShader = shader;
				GameEngine::allShaders.push_back(shader);
				//shader->Bind();
			}
		}
		else
		{
			shader = GameEngine::currentShader;
			//shader->Bind();

		}
	}

	if (shader == nullptr) {
		std::cout << "ERORR: Shader missing!!" << std::endl;
	}
}

void Material::SetDiffuseTexture(unsigned int textureid, Texture::TheOtherType num_type)
{
	diffuseColour = glm::vec3(1.0, 1.0, 1.0);
	Texture diffTex(textureid);
	diffTex.numType = num_type;
	diffTex.type = "texture_diffuse";
	textures.push_back(diffTex);
	
}

void Material::SetDiffuseTexture(std::string diffuseTexName, Texture::TheOtherType num_type, bool transparent)
{
	diffuseColour = glm::vec3(1.0, 1.0, 1.0);
	diffTexName = diffuseTexName;
	Texture diffTex(diffuseTexName, transparent);
	diffTex.numType = num_type;
	diffTex.type = "texture_diffuse";
	textures.push_back(diffTex);
	
}

void Material::SetDiffuseTexture(std::vector<glm::vec4> imageData, Texture::TheOtherType num_type, bool transparent)
{
	diffuseColour = glm::vec3(1.0, 1.0, 1.0);
	diffTexName = "";
	Texture diffTex(imageData, transparent);
	diffTex.numType = num_type;
	diffTex.type = "texture_diffuse";
	textures.push_back(diffTex);
	
}

//void Material::SetDiffuseTexture(std::vector<std::string> diffuseTexNames)
//{
//	diffuseColour = glm::vec3(1.0, 1.0, 1.0);
//	for (int i = 0; i < diffuseTexNames.size(); i++) {
//		diffTexName = diffuseTexNames[i];
//		Texture diffTex(diffuseTexNames[i], true);
//		diffTex.type = "texture_diffuse";
//		textures.push_back(diffTex);
//	}
//}

void Material::SetSpecularTexture(std::string specTexName)
{
	Texture specTex(specTexName);
	specTex.type = "texture_specular";
	textures.push_back(specTex);
}

void Material::SetEmissionTexture(std::string emissionTexName, Emission_Behaviour behave)
{
	Texture emTex(emissionTexName);
	emTex.type = "texture_emission";
	textures.push_back(emTex);
	emissionAmplifier = 1;
	emissionBehaviour = behave;
}

Material::Material(const char* vshaderPath, const char* fshaderPath, float shine, glm::vec3 spec)
{ 
	SetShader(vshaderPath, fshaderPath);
	shininess = shine;
	specularColour = spec;
}

Material::Material(std::vector<Texture> tex, const char * vshaderPath, const char * fShaderPath, float shine, glm::vec3 spec)
{
	SetShader(vshaderPath, fShaderPath);
	textures = tex;
	this->diffuseColour = glm::vec3(1.0, 1.0, 1.0);
	shininess = shine;
	specularColour = spec;
}

Material::Material(glm::vec3 colour, const char* vshaderPath, const char* fshaderPath, float shine, glm::vec3 spec)
{
	SetShader(vshaderPath, fshaderPath);
	
	diffuseColour = glm::vec3(colour.r, colour.g, colour.b);
	specularColour = spec;
	shininess = shine;
}


Material::~Material()
{
	
	delete shader;
}

void Material::BindShader()
{
	shader->Bind();
}

void Material::BindTextures()
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	unsigned int emissionNr = 1;

	
	shader->SetUniform1f("material.shininess", shininess);
	shader->SetUniform1i("material.emissionAmp", emissionAmplifier);
	shader->SetUniform1i("material.emissionBehaviour", emissionBehaviour);
	shader->SetUniformVec3("material.diffuseCol", diffuseColour);
	shader->SetUniformVec3("material.specularCol", specularColour);
	for (unsigned int i = 0; i < this->textures.size(); i++) {


		std::string number;
		std::string name = this->textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(textures[i].numType);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_emission")
			number = std::to_string(emissionNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		textures[i].Bind(i);
		shader->SetUniform1i("material."+(name + number), i);
		//textures[i].Unbind(i);
		
	}
	

}

void Material::UnindTextures()
{
	for (unsigned int i = 0; i < this->textures.size(); i++) {
		textures[i].Unbind(i);
	}
}

void Material::BindUniforms()
{
	//Set the uniforms within the shader and the program
	shader->Bind();
	shader->SetUniformVec3("colour", diffuseColour);
	
	/*shader->SetUniformVec3("u_Material.ambientColour", ambientColour);
	shader->SetUniformVec3("u_Material.DiffuseColour", diffuseColour);
	shader->SetUniformVec3("u_Material.SpecularColour", specularColour);
	shader->SetUniform1f("u_Material.Dissolve", dissolve);*/
//	shader->SetUniform1i("u_Material.illum", illum);
}
