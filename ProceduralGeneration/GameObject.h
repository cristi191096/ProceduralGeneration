#pragma once


#include "Transform.h"
#include "Model.h"
#include <map>
#include <vector>

class GameObject
{
protected:
	std::vector<Vertex> verts;
	std::vector<unsigned int> inds;


public:
	static bool GameObject::debugMode;
	static std::map<int, bool> GameObject::specialKeys;
	static std::map<char, bool> GameObject::keys;
	std::string tag;
	Transform* transform;
	Model* model = nullptr;
	bool waterObject = false;

	GameObject(glm::vec3, std::string tag);
	virtual ~GameObject();

	virtual unsigned int setupDrawing(unsigned int);

	virtual void draw() = 0;

	virtual void start() = 0;
	virtual void update(int deltaTime) = 0;

	//	void LoadMesh(std::string path);

};

