#pragma once

/*Holds Reflection and Refraction frame buffers used for water*/

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	unsigned int getRefractionDepthTexture();
	unsigned int getRefractionTexture();
	unsigned int getReflectionTexture();
	void unbindCurrentFrameBuffer();

	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	

private:
	unsigned int reflectionFrameBuffer;
    unsigned int reflectionTexture;
    unsigned int reflectionDepthBuffer;

    unsigned int refractionFrameBuffer;
    unsigned int refractionTexture;
    unsigned int refractionDepthTexture;

	void initialiseReflectionFrameBuffer();
	void initialiseRefractionFrameBuffer();
	void bindFrameBuffer(int frameBuffer, int width, int height);
	unsigned int createFrameBuffer();
	unsigned int createTextureAttachment(int width, int height);
	unsigned int createDepthTextureAttachment(int width, int height);
	unsigned int createDepthBufferAttachment(int width, int height);
};

