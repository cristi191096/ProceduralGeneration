#include "Transform.h"
#include "Material.h"


void Transform::SetModelMat()
{
	model = mPosition * mRot * mScale;
}

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc)
{
	this->position = pos;
	this->rotation = rot;
	this->scale = sc;
	this->Translate();
}

void Transform::Translate()
{
	mPosition = glm::mat4(1.0);
	mPosition = glm::translate(mPosition,
		glm::vec3(this->position.x, this->position.y, this->position.z)); 
}

void Transform::Rotate()
{
	/*if (rotation.x == 0 && rotation.y == 0 && rotation.z == 0) {
		return;
	}*/

	mRot = glm::mat4(1.0);
	//mPosition = glm::mat4(1.0);
	mPosition = glm::translate(mPosition,
		glm::vec3(-position.x, -position.y, -position.z));
	mRot = glm::rotate(mRot, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	mRot = glm::rotate(mRot, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	mRot = glm::rotate(mRot, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	mPosition = glm::translate(mPosition,
		glm::vec3(position.x, position.y, position.z));
	
}

void Transform::Scale()
{
	mScale = glm::mat4(1.0);
	mScale = glm::scale(mScale, glm::vec3(this->scale.x, this->scale.y, this->scale.z));
	
}

void Transform::UpdateShader(Shader * shader)
{
	Translate();
	Rotate();
	Scale();
	SetModelMat();
	shader->Bind();
	shader->SetUniformMat4("model", model);
	shader->Unbind();
	mRot = glm::mat4(1.0);
	mScale = glm::mat4(1.0);
	mPosition = glm::mat4(1.0);
	model = glm::mat4(1.0);
}

Transform::~Transform()
{
}
