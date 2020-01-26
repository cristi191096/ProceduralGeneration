#pragma once

#include <GL\glew.h>
#include <string>
#include <vector>
#include <glm.hpp>


class Texture
{
	unsigned int id;
	unsigned char* localBuffer;
	int width, height, BPP;


public:
	enum TheOtherType {
		None = 0,
		Grass, Sand, Snow, Rock, Water, Cloud, WaterReflection, WaterRefraction
	};

	TheOtherType numType;
	std::string filePath;
	std::string type; // like the name in the shader (eg. "texture_diffuse", "texture_specular")
	//sampler2D for the shader. 
	Texture(unsigned int texId) { id = texId; }
	Texture(const std::string& path ,bool enableTransparency = false);
	Texture(std::vector<std::string> faces);
	Texture(std::vector<glm::vec4> imageData, bool enableTransparency);
	~Texture();

	void Bind(unsigned int slot = 0, GLenum glTarget = GL_TEXTURE_2D) const;
	
	void Unbind(unsigned int slot = 0) const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	

private:

};

