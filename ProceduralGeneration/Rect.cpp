#include "Rect.h"

#include "GameEngine.h"




Rect::Rect( glm::vec3 pos, std::string tag) : GameObject(pos, tag)
{
	

	this->model = new Model();
	//this->model->AddNewMesh(verts, inds);

	GameEngine::addGameObject(this);
}

void Rect::draw()
{
	//transform->Rotate(90, glm::vec3(1.0, 0.0, 0.0));
	//transform->Scale(glm::vec3(0.0, rotAngle, 0.0));

	transform->UpdateShader(this->model->meshes[0].meshMaterial->shader);
	
	this->model->Draw();

}

void Rect::start()
{
	
}

void Rect::update(int deltaTime)
{

	//transform->Rotate(45 , glm::vec3(0, 0, 1));
	//transform->SendModelMatrix(this->mesh->meshMaterial->shader);
}

void Rect::AddRect(float width, float height)
{
		Vertex v0 = {
		glm::vec3(transform->position.x-(width/2), transform->position.y, transform->position.z - height / 2),
		glm::vec3(0,1,0),
		glm::vec2(0.0, 0.0),
		glm::vec3(1.0,0.0,0.0),
		glm::vec3(0.0,0.0,1.0)};
	verts.push_back(v0);
	Vertex v1 = {
		glm::vec3(transform->position.x + width / 2, transform->position.y, transform->position.z - height / 2),
		glm::vec3(0,1,0),
		glm::vec2(1.0, 0.0),
		glm::vec3(1.0,0.0,0.0),
		glm::vec3(0.0,0.0,1.0) };
	verts.push_back(v1);
	Vertex v2 = {
		glm::vec3(transform->position.x + width / 2, transform->position.y, transform->position.z + height / 2),
		glm::vec3(0,1,0),
		glm::vec2(1.0, 1.0),
		glm::vec3(1.0,0.0,0.0),
		glm::vec3(0.0,0.0,1.0) };
	verts.push_back(v2);
	Vertex v3 = {
		glm::vec3(transform->position.x - width / 2, transform->position.y, transform->position.z + height / 2),
		glm::vec3(0,1,0),
		glm::vec2(0.0, 1.0),
		glm::vec3(1.0,0.0,0.0),
		glm::vec3(0.0,0.0,1.0) };
	verts.push_back(v3);
	inds.push_back(0);
	inds.push_back(1);
	inds.push_back(2);
	inds.push_back(2);
	inds.push_back(3);
	inds.push_back(0);
	this->model->AddNewMesh(verts, inds);
	verts.clear();
	inds.clear();
}

//void Rect::SetTexture(std::string texFile)
//{
//	for (unsigned int i = 0; i < model->meshes.size(); i++) {
//		this->model->meshes[i].meshMaterial->SetDiffuseTexture(texFile);
//	}
//}



Rect::~Rect()
{
		delete transform;
	delete model;
}
