#pragma once
#include "Light.h"
#include "GameObject.h"

class PointLight :	public Light, public GameObject
{
	
	float Kc = 1.0;
	float Kl = 0.09;
	float Kq = 0.032;
	int lightId;

public:
	float radius;
	float intensity;
	PointLight(glm::vec3 pos, glm::vec3 diff,float intensity, float radius = 2, glm::vec3 amb = glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3 spec = glm::vec3(1.0f, 1.0f, 1.0f));
	~PointLight();
	void BindUniforms(Shader* shader) const;
	void draw();
	void start();
	void update(int deltaTime);
	void DebugShape(glm::vec3 centre, float width, float height, float length);

};

