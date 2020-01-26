#include "PointLight.h"
#include "GameEngine.h"

PointLight::PointLight(glm::vec3 pos, glm::vec3 diff,float intensity, float radius, glm::vec3 amb, glm::vec3 spec) : Light(amb, diff, spec), GameObject(pos, "PointLight")
{
	this->intensity = intensity;
	this->radius = radius;
	Kl = 2 / radius;
	Kq = 1 / (radius * radius);
	DebugShape(pos, 0.2, 0.2, 0.2);
	model = new Model();
	model->AddNewMesh(verts, inds);
	model->meshes[0].meshMaterial->diffuseColour = diff;
	model->meshes[0].meshMaterial->BindUniforms();
	GameEngine::addGameObject(this);
	lightId = GameEngine::pointLights;
	GameEngine::pointLights++;
	//id = GameEngine::pointLights;
}

PointLight::~PointLight()
{
}

void PointLight::BindUniforms(Shader * shader) const
{
	shader->Bind();
	shader->SetUniformVec3("pointLight[" + std::to_string(lightId) +"].position", glm::vec3(transform->position.x, transform->position.y,transform->position.z)); 
	shader->SetUniformVec3("pointLight[" + std::to_string(lightId) +"].ambient", ambientColour);
	shader->SetUniformVec3("pointLight[" + std::to_string(lightId) +"].diffuse", diffuseColour);
	shader->SetUniformVec3("pointLight[" + std::to_string(lightId) +"].specular", specularColour);
	shader->SetUniform1f("pointLight[" + std::to_string(lightId) + "].intensity", intensity);
	shader->SetUniform1f("pointLight[" + std::to_string(lightId) + "].constant", Kc);
	shader->SetUniform1f("pointLight[" + std::to_string(lightId) + "].linear", Kl);
	shader->SetUniform1f("pointLight[" + std::to_string(lightId) + "].quadratic", Kq);
}										 

void PointLight::draw()
{
	
	transform->UpdateShader(this->model->meshes[0].meshMaterial->shader);
	BindUniforms(this->model->meshes[0].meshMaterial->shader);
	if (GameObject::debugMode) {
		this->model->Draw();
	}
}

void PointLight::start()
{
}

void PointLight::update(int deltaTime)
{
}

void PointLight::DebugShape(glm::vec3 centre, float width, float height, float length)
{
	//Front
	Vertex v0 = {
		glm::vec3(
			centre.x - width / 2,
			centre.y - height / 2,
			centre.z + length / 2),
	};
	verts.push_back(v0);
	Vertex v1 = {
		glm::vec3(
			centre.x + width / 2,
			centre.y - height / 2,
			centre.z + length / 2),
	};
	verts.push_back(v1);
	Vertex v2 = {
		glm::vec3(
			centre.x + width / 2,
			centre.y + height / 2,
			centre.z + length / 2),
	};
	verts.push_back(v2);
	Vertex v3 = {
		glm::vec3(
			centre.x - width / 2,
			centre.y + height / 2,
			centre.z + length / 2),
	};
	verts.push_back(v3);
	//Back
	Vertex v4 = {
		glm::vec3(
			centre.x - (width / 2),
			centre.y - (height / 2),
			centre.z - length / 2),
	};
	verts.push_back(v4);
	Vertex v5 = {
		glm::vec3(
			centre.x + width / 2,
			centre.y - height / 2,
			centre.z - length / 2),
	};
	verts.push_back(v5);
	Vertex v6 = {
		glm::vec3(
			centre.x + width / 2,
			centre.y + height / 2,
			centre.z - length / 2),
	};
	verts.push_back(v6);
	Vertex v7 = {
		glm::vec3(
			centre.x - width / 2,
			centre.y + height / 2,
			centre.z - length / 2),
	};
	verts.push_back(v7);

	//Front
	inds.push_back(0);	inds.push_back(1);	inds.push_back(2);
	inds.push_back(2);	inds.push_back(3);	inds.push_back(0);
	//Left
	inds.push_back(4);	inds.push_back(7);	inds.push_back(3);
	inds.push_back(3);	inds.push_back(0);	inds.push_back(4);
	//Back
	inds.push_back(5);	inds.push_back(4);	inds.push_back(7);
	inds.push_back(7);	inds.push_back(6);	inds.push_back(5);
	//Right
	inds.push_back(6);	inds.push_back(2);	inds.push_back(1);
	inds.push_back(1);	inds.push_back(5);	inds.push_back(6);
	//Top
	inds.push_back(6);	inds.push_back(7);	inds.push_back(3);
	inds.push_back(3);	inds.push_back(2);	inds.push_back(6);
	//Bottom
	inds.push_back(1);	inds.push_back(0);	inds.push_back(4);
	inds.push_back(4);	inds.push_back(5);	inds.push_back(1);

}

