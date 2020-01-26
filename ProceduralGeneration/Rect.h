#pragma once
#include "GameObject.h"
#include <vector>

class Rect : public GameObject
{
	float rotAngle;
public:
	Rect(glm::vec3 pos, std::string tag);
	void draw();
	void start();
	void update(int deltaTime);
	void AddRect(float width, float height);
	//void SetTexture(std::string texFile);

	~Rect();
};

