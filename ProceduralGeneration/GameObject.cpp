#include "GameObject.h"

#include <iostream>

bool GameObject::debugMode;
std::map<int, bool> GameObject::specialKeys;
std::map<char, bool> GameObject::keys;

GameObject::GameObject(glm::vec3 pos, std::string tag)
{
	this->tag = tag;
	this->transform = new Transform(pos, glm::vec3(0,0,0), glm::vec3(1,1,1));

	
}


GameObject::~GameObject()
{
	delete transform;
	delete model;
	
}

unsigned int GameObject::setupDrawing(unsigned int)
{
	return 0;
}




