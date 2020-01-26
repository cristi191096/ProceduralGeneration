#pragma once
#include "Light.h"
#include "GameObject.h"

class DirectionalLight :	public Light, public GameObject
{
	glm::vec3 direction;
public:
	DirectionalLight(glm::vec3 diff, glm::vec3 dir = glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3 pos = glm::vec3(0,3,0), glm::vec3 amb = glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3 spec = glm::vec3(1.0f, 1.0f, 1.0f));
	~DirectionalLight();
	void BindUniforms(Shader* shader) const;
	void draw();
	void start();
	void update(int deltaTime);
	void DebugShape(glm::vec3 centre, float width, float height, float length);

};

