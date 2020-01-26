#pragma once
#include <glm.hpp>
#include "gtc\matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
class Shader;

class Transform
{
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 mScale = glm::mat4(1.0);
	glm::mat4 mRot = glm::mat4(1.0);
	glm::mat4 mPosition = glm::mat4(1.0);

	void SetModelMat();
	void Translate();
	void Rotate();
	void Scale();

public:

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc);
	void UpdateShader(Shader* shader);
	~Transform();
};

