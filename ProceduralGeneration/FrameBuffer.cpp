#include "FrameBuffer.h"
#include <GL\glew.h>
#include "GameEngine.h"


   static int REFLECTION_WIDTH = 320;
   static int REFLECTION_HEIGHT = 180;
   
   static int REFRACTION_WIDTH = 1280;
   static int REFRACTION_HEIGHT = 720;

FrameBuffer::FrameBuffer()
{
	   initialiseReflectionFrameBuffer();
	   initialiseRefractionFrameBuffer();
}


FrameBuffer::~FrameBuffer()
{
}


 void FrameBuffer::bindReflectionFrameBuffer() {//call before rendering to this FBO
        bindFrameBuffer(reflectionFrameBuffer,REFLECTION_WIDTH,REFLECTION_HEIGHT);
    }
     
   void FrameBuffer::bindRefractionFrameBuffer() {//call before rendering to this FBO
        bindFrameBuffer(refractionFrameBuffer,REFRACTION_WIDTH,REFRACTION_HEIGHT);
    }



  void FrameBuffer::unbindCurrentFrameBuffer() {//call to switch to default frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, GameEngine::SCREEN_WIDTH, GameEngine::SCREEN_HEIGHT);
    }
 
    unsigned int FrameBuffer::getReflectionTexture() {//get the resulting texture
        return reflectionTexture;
    }
     
    unsigned int FrameBuffer::getRefractionTexture() {//get the resulting texture
        return refractionTexture;
    }
     
   unsigned int FrameBuffer::getRefractionDepthTexture(){//get the resulting depth texture
        return refractionDepthTexture;
    }
 
     void FrameBuffer::initialiseReflectionFrameBuffer() {
        reflectionFrameBuffer = createFrameBuffer();
        reflectionTexture = createTextureAttachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);
        reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);
        unbindCurrentFrameBuffer();
    }
     
     void FrameBuffer::initialiseRefractionFrameBuffer() {
        refractionFrameBuffer = createFrameBuffer();
        refractionTexture = createTextureAttachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
        refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
        unbindCurrentFrameBuffer();
    }



     void FrameBuffer::bindFrameBuffer(int frameBuffer, int width, int height){
        glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glViewport(0, 0, width, height);
    }
 
     unsigned int FrameBuffer::createFrameBuffer() {
		unsigned int frameBuffer;
		 glGenFramebuffers(1, &frameBuffer);
        //generate name for frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        //create the framebuffer
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        //indicate that we will always render to color attachment 0
        return frameBuffer;
    }
 
     unsigned int FrameBuffer::createTextureAttachment( int width, int height) {
		 unsigned int texture;
		 glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                texture, 0);
        return texture;
    }
     
     unsigned int FrameBuffer::createDepthTextureAttachment(int width, int height){
		unsigned int texture;
		glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
                0, GL_DEPTH_COMPONENT, GL_FLOAT,  NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                texture, 0);
        return texture;
    }
 
    unsigned int FrameBuffer::createDepthBufferAttachment(int width, int height) {
		unsigned int depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,
                height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER, depthBuffer);
        return depthBuffer;
    }
