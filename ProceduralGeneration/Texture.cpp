#include "Texture.h"
#include "stb_image.h"
#include <iostream>



Texture::Texture(const std::string& path, bool enableTransparency)
	: id(0), filePath(path), localBuffer(nullptr),
	width(0), height(0), BPP(0)
{
	

	//if (enableTransparency) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//}
	//else
	//{
	//	glDisable(GL_BLEND);
	//}

	stbi_set_flip_vertically_on_load(0);
	localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &id);
	
		if (localBuffer) {

			GLenum format;
			if (BPP == 1) format = GL_RGBA;
			else if (BPP == 3) format = GL_RGBA;
			else if (BPP == 4) format = GL_RGBA;
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, localBuffer);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(localBuffer);
		}
		else
		{
			std::cout << "Texture at path: " << path << " failed to load!" << std::endl;
			stbi_image_free(localBuffer);
		}
	
		glDisable(GL_TEXTURE_2D);
}

Texture::Texture(std::vector<std::string> faces)
	: id(0), localBuffer(nullptr),
	width(0), height(0), BPP(0)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	for (unsigned int i = 0; i < faces.size(); i++) {
		localBuffer = stbi_load(faces[i].c_str(), &width, &height, &BPP, 0); //Read texture file
		if (localBuffer) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer);
			
			stbi_image_free(localBuffer);
		}
		else
		{
			std::cout << "Failed to load skybox face: " << faces[i] << std::endl;
			stbi_image_free(localBuffer);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


}

Texture::Texture(std::vector<glm::vec4> imageData, bool enableTransparency) {
	float* imageLoader = new float[imageData.size() * 4]();

	for (int i = 0; i < imageData.size(); i++) {
		imageLoader[i * 4] = (float)imageData[i].r;
		imageLoader[i * 4 + 1] = (float)imageData[i].g;
		imageLoader[i * 4 + 2] = (float)imageData[i].b;
		imageLoader[i * 4 + 3] = (float)imageData[i].a;
	}

	if (enableTransparency) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	float sizeImage = std::sqrt(imageData.size());

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeImage, sizeImage, 0, GL_RGBA, GL_FLOAT, imageLoader);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	glDisable(GL_TEXTURE_2D);
}


Texture::~Texture()
{
	/*if (id) 
	{
		glDeleteTextures(1, &id);
		id = 0;
	}*/
}

void Texture::Bind(unsigned int slot, GLenum glTarget) const
{

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(glTarget, id);

}


void Texture::Unbind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

 