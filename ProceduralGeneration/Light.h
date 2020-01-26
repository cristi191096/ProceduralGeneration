#pragma once
#include "glm.hpp"
#include "Shader.h"

class Light
{

public:
	glm::vec3 ambientColour;
	glm::vec3 diffuseColour;
	glm::vec3 specularColour;
	Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
	~Light();
	virtual void BindUniforms(Shader* shader) const = 0;
};

