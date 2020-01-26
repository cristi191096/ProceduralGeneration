#include "DirectionalLight.h"
#include "GameEngine.h"


DirectionalLight::DirectionalLight(glm::vec3 diff, glm::vec3 dir, glm::vec3 pos, glm::vec3 amb, glm::vec3 spec) : Light(amb, diff, spec), GameObject(pos, "DirectionalLight")
{
	direction = glm::normalize(dir);
	DebugShape(pos, 0.2, 0.2, 0.2);
	model = new Model();
	model->AddNewMesh(verts, inds);
	model->meshes[0].meshMaterial->diffuseColour = diff;
	model->meshes[0].meshMaterial->BindUniforms();
	GameEngine::addGameObject(this);
}



DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::BindUniforms(Shader * shader) const
{
	shader->Bind();
	shader->SetUniformVec3("dirLight.direction", glm::vec3(direction.x, direction.y, direction.z));
	shader->SetUniformVec3("dirLight.ambient", ambientColour);
	shader->SetUniformVec3("dirLight.diffuse", diffuseColour);
	shader->SetUniformVec3("dirLight.specular", specularColour);
}

void DirectionalLight::draw()
{
	if (transform->rotation.x != 0 || transform->rotation.y != 0 || transform->rotation.z != 0) {
		direction = transform->rotation;
		direction = glm::normalize(direction);
	}
	transform->UpdateShader(this->model->meshes[0].meshMaterial->shader);
	for (int i = 0; i < GameEngine::allShaders.size(); i++) {
		BindUniforms(GameEngine::allShaders[i]);
	}
	if (GameObject::debugMode) {
		this->model->Draw();
	}
}

void DirectionalLight::start()
{
	
}

void DirectionalLight::update(int deltaTime)
{

}

void DirectionalLight::DebugShape(glm::vec3 centre, float width, float height, float length)
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
