#include "Light.h"


Light::Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
{
	ambientColour = amb;
	diffuseColour = diff;
	specularColour = spec;
}

Light::~Light()
{
}
